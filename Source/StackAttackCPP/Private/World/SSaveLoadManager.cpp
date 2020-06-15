// Fill out your copyright notice in the Description page of Project Settings.


#include "SSaveLoadManager.h"
#include "SSaving.h"
#include "SItemList.h"
#include "SRoom.h"
#include "SFloor.h"
#include "SCoin.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "SPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"

// Sets default values
ASSaveLoadManager::ASSaveLoadManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASSaveLoadManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASSaveLoadManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Save game session
void ASSaveLoadManager::SaveGame()
{
	//Create save object
	USSaving *Save = Cast<USSaving>(UGameplayStatics::CreateSaveGameObject(USSaving::StaticClass()));

	//Save room
	ASRoom *Room = Cast<ASRoom>(UGameplayStatics::GetActorOfClass(GetWorld(), TSubclassOf<ASRoom>(ASRoom::StaticClass())));
	if (Room) {
		Save->SaveRoomStruct(Room->SaveRoom());
	}

	//Save Player 
	ASCharacter *Player = Cast<ASCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Player) {
		Save->SaveCharacter(Player->SaveCharacter());
	}

	//Save Player State
	ASPlayerState *PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<ASPlayerState>();
	if (PlayerState) {
		Save->SavePlayerState(PlayerState->SavePlayerState());
	}

	//Save all Items
	for (TActorIterator<ASItemBase> It(GetWorld()); It; ++It) {

		ASItemBase *Item = Cast<ASItemBase>(*It);
		if (Item) {
			Save->AddItem(Item->SaveItem());
		}
	}

	//Save all Coins
	for (TActorIterator<ASCoin> It(GetWorld()); It; ++It) {

		ASCoin *Coin = Cast<ASCoin>(*It);
		if (Coin) {
			Save->AddCoin(Coin->SaveCoin());
		}
	}

	//Save all Cranes
	for (TActorIterator<ASCrane> It(GetWorld()); It; ++It) {

		ASCrane *Crane = Cast<ASCrane>(*It);
		if (Crane) {
			Save->AddCrane(Crane->SaveCrane());
		}
	}

	//Save Game to slot
	UGameplayStatics::SaveGameToSlot(Save, "New", 0);
}


//Load game session
void ASSaveLoadManager::LoadGame(FString SlotName)
{
	//Get save file
	USSaving *Load = Cast<USSaving>(UGameplayStatics::CreateSaveGameObject(USSaving::StaticClass()));
	
	if (Load) {
		Load = Cast<USSaving>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

		TArray<class ASItemBase *> FreshItemArr;
		TArray<class ASItemBase *> AttachedItemArr;

		//Load the room
		ASRoom *Room = Cast<ASRoom>(UGameplayStatics::GetActorOfClass(GetWorld(), TSubclassOf<ASRoom>(ASRoom::StaticClass())));
		if (Room) {
			Room->LoadRoom(Load->GetRoomStruct());
		}

		//Load and spawn all Items
		const TArray<FSaveItemStruct> &ItemStruct = Load->GetItemStructArray();

		for (size_t i = 0; i < ItemStruct.Num(); i++) {

			ASItemBase *Item = Room->SpawnItem(ItemStruct[i].ItemLocation, ItemStruct[i].ItemClass);

			if (Item) {
				Item->LoadItem(ItemStruct[i]);

				if (Item->GetJustSpawned()) {
					FreshItemArr.Add(Item);
				}
				else if (Item->GetAttachedToCrane())
				{
					AttachedItemArr.Add(Item);
				}
			}
		}

		//Load and spawn all cranes
		const TArray<FSaveCraneStruct> &CraneStruct = Load->GetCraneStructArray();

		for (TActorIterator<ASCrane> It(GetWorld()); It; ++It) {
			if (CraneStruct.Num() >= 0) {
				int32 i = It.GetProgressNumerator() - 1;
				It->LoadCrane(CraneStruct[i]);
				if (!It->IsFree()) {
					ASItemBase *item = nullptr;
					if (!It->IsHasAttachedItem()) {
						for (auto f : FreshItemArr) {
							if (CraneStruct[i].LocationMoveTo.X == f->GetActorLocation().X
								&& CraneStruct[i].LocationMoveTo.Y == f->GetActorLocation().Y)
							{
								item = f;
							}
						}
						It->DeliverItem(CraneStruct[i].LocationToDetachItem, item);
					}
					else
					{
						for (auto f : AttachedItemArr) {
							if (CraneStruct[i].CraneLocation.X == f->GetActorLocation().X
								&& CraneStruct[i].CraneLocation.Y == f->GetActorLocation().Y)
							{
								item = f;
							}
						}
						It->HalfWay(item);
					}
					Room->GetFloor()->SetGridBusy(CraneStruct[i].LocationToDetachItem);
				}
			}
		}

		//Load all coins
		const TArray<FCoinSaveStruct> &CoinStruct = Load->GetCoinStructArray();

		for (size_t i = 0; i < CoinStruct.Num(); i++) {

			ASCoin *Coin = GetWorld()->SpawnActor<ASCoin>(ASCoin::StaticClass(), CoinStruct[i].CoinLocation, CoinStruct[i].RotationSpeed);
			Coin->LoadCoin(CoinStruct[i]);
		}

		//Load Player
		Cast<ASCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter())->LoadCharacter(Load->GetCharacterStruct());

		//Load PlayerState
		GetWorld()->GetFirstPlayerController()->GetPlayerState<ASPlayerState>()->LoadPlayerState(Load->GetPlayerStateStruct());

		//GetWorld()->GetFirstPlayerController<ACPlayerController>()->UpdatePoints(Load->GetPlayerStateStruct().ItemStruct.points);
	}
}

