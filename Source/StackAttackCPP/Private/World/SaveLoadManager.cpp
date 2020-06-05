// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveLoadManager.h"
#include "Saving.h"
#include "ItemList.h"
#include "Room.h"
#include "Floor.h"
#include "MainCharacter.h"
#include "MainPlayerState.h"
#include "CPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "MainCharacter.h"
#include "EngineUtils.h"

// Sets default values
ASaveLoadManager::ASaveLoadManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASaveLoadManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASaveLoadManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Save game session
void ASaveLoadManager::SaveGame()
{
	//Create save object
	USaving *Save = Cast<USaving>(UGameplayStatics::CreateSaveGameObject(USaving::StaticClass()));

	//Save room
	ARoom *Room = Cast<ARoom>(UGameplayStatics::GetActorOfClass(GetWorld(), TSubclassOf<ARoom>(ARoom::StaticClass())));
	if (Room) {
		Save->SaveRoomStruct(Room->SaveRoom());
	}

	//Save Player 
	AMainCharacter *Player = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Player) {
		Save->SaveCharacter(Player->SaveCharacter());
	}

	//Save Player State
	AMainPlayerState *PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AMainPlayerState>();
	if (PlayerState) {
		Save->SavePlayerState(PlayerState->SavePlayerState());
	}

	//Save all Items
	for (TActorIterator<AItem> It(GetWorld()); It; ++It) {

		AItem *Item = Cast<AItem>(*It);
		if (Item) {
			Save->AddItem(Item->SaveItem());
		}
	}

	//Save all Cranes
	for (TActorIterator<ACrane> It(GetWorld()); It; ++It) {

		ACrane *Crane = Cast<ACrane>(*It);
		if (Crane) {
			Save->AddCrane(Crane->SaveCrane());
		}
	}

	//Save Game to slot
	UGameplayStatics::SaveGameToSlot(Save, "New", 0);
}


//Load game session
void ASaveLoadManager::LoadGame(FString SlotName)
{
	//Get save file
	USaving *Load = Cast<USaving>(UGameplayStatics::CreateSaveGameObject(USaving::StaticClass()));
	
	if (Load) {
		Load = Cast<USaving>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

		//Load the room
		ARoom *Room = Cast<ARoom>(UGameplayStatics::GetActorOfClass(GetWorld(), TSubclassOf<ARoom>(ARoom::StaticClass())));
		if (Room) {
			Room->LoadRoom(Load->GetRoomStruct());
		}

		//Load and spawn all Items
		const TArray<FSaveItemStruct> &ItemStruct = Load->GetItemStructArr();

		for (size_t i = 0; i < ItemStruct.Num(); i++) {

			AItem *Item = Room->SpawnItem(ItemStruct[i].ItemLocation, ItemStruct[i].ItemClass);

			if (Item) {
				Item->LoadItem(ItemStruct[i]);

				if (Item->GetFresh()) {
					FreshItemArr.Add(Item);
				}
				else if (Item->GetAttached())
				{
					AttachedItemArr.Add(Item);
				}
				else
				{
					Item->UpdateOverlaps();
				}
			}
		}

		//Load and spawn all cranes
		const TArray<FSaveCraneStruct> &CraneStruct = Load->GetCraneStructArr();

		for (TActorIterator<ACrane> It(GetWorld()); It; ++It) {
			if (CraneStruct.Num() >= 0) {
				int32 i = It.GetProgressNumerator() - 1;
				It->LoadCrane(CraneStruct[i]);
				if (!It->IsFree()) {
					AItem *item = nullptr;
					if (!It->IsAttached()) {
						for (auto f : FreshItemArr) {
							if (CraneStruct[i].MoveToLoc.X == f->GetActorLocation().X
								&& CraneStruct[i].MoveToLoc.Y == f->GetActorLocation().Y)
							{
								item = f;
							}
						}
						It->DeliverItem(CraneStruct[i].DetachLoc, item);
					}
					else
					{
						for (auto f : AttachedItemArr) {
							if (CraneStruct[i].RelLocCrane.X == f->GetActorLocation().X
								&& CraneStruct[i].RelLocCrane.Y == f->GetActorLocation().Y)
							{
								item = f;
							}
						}
						It->HalfWay(item);
					}
					Room->GetFloor()->SetGridBusy(CraneStruct[i].DetachLoc);
				}
			}
		}

		//Load Player
		Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter())->LoadCharacter(Load->GetCharacterStruct());

		//Load PlayerState
		GetWorld()->GetFirstPlayerController()->GetPlayerState<AMainPlayerState>()->LoadPlayerState(Load->GetPlayerStateStruct());
		GetWorld()->GetFirstPlayerController<ACPlayerController>()->UpdatePoints(Load->GetPlayerStateStruct().points);
	}
}

