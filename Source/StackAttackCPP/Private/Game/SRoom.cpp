// Fill out your copyright notice in the Description page of Project Settings.


#include "SRoom.h"
#include "SItemList.h"
#include "SCrane.h"
#include "SFloor.h"
#include "SCoin.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "UObject/ConstructorHelpers.h"


ASRoom::ASRoom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ASRoom::InitializeRoom(int32 roomWidth)
{
	RoomWidth = roomWidth;

	SpawnVecEdge = GridSize * (RoomWidth);
	SpawnVecHeight = GridSize * (StackHeight + 3.5);

	CraneAmountMax = RoomWidth / 3;

	DifficultIncreaseMax = CraneAmountMax * 5;

	//Create a floor actor
	Floor = GetWorld()->SpawnActor<ASFloor>(ASFloor::StaticClass(), FVector(0), FRotator::ZeroRotator);
	if (Floor) {
		Floor->InitializeFloor(RoomWidth, GridSize, StackHeight);
	}

	//Start searching a free crane timer
	GetWorld()->GetTimerManager().SetTimer(CraneSearchTHandle, this, &ASRoom::SearchFreeCraneAndSpawnItem, 1.0f, true, 2.0f);

	//Start increase the level difficult timer
	GetWorld()->GetTimerManager().SetTimer(DifficultIncreaseHandle, this, &ASRoom::IncreaseDifficult, 30.0f, true, 0.1f);

	//Start coin spawning
	GetWorld()->GetTimerManager().SetTimer(CoinSpawnerHandle, this, &ASRoom::CoinSpawner, 1.0f, false);
	
}


// Called when the game starts or when spawned
void ASRoom::BeginPlay()
{	
	Super::BeginPlay();
}

// Called every frame
void ASRoom::Tick(float DeltaTime)
{

}

//Increase the difficult periodically
void ASRoom::IncreaseDifficult()
{
	if (DifficultIncreaseCurrent != DifficultIncreaseMax) {
		if (DifficultIncreaseCurrent % 5 == 0 && CraneAmountCurrent != DifficultIncreaseMax) {
			SpawnNewCrane();
			for (size_t i = 0; i < CraneAmountCurrent - 1; i++)
			{
				CraneArray[i]->ResetSpeed();
			}
		}
		else
		{
			for (size_t i = 0; i < CraneAmountCurrent; i++)
			{
				CraneArray[i]->IncreaseSpeed();
			}
		}
	}
	else {
		//If the level difficult is max - clear timer
		GetWorld()->GetTimerManager().ClearTimer(DifficultIncreaseHandle);
	}
	DifficultIncreaseCurrent++;
}


ASCrane* ASRoom::SpawnNewCrane()
{
	if (CraneAmountCurrent != CraneAmountMax) {
		ASCrane *crane = GetWorld()->SpawnActor<ASCrane>(ASCrane::StaticClass(), FVector(0, 0, (StackHeight + 3) * GridSize),
			FRotator::ZeroRotator);
		if (crane) {
			CraneArray.Add(crane);
			crane->InitCrane(FVector(150, 350, (StackHeight + 5) * GridSize), RoomWidth, GridSize);
			CraneAmountCurrent++;
			return crane;
		}
	}
	return nullptr;
}


//Search for a free crane. If found - spawn a new item and deliver it
void ASRoom::SearchFreeCraneAndSpawnItem()
{
	for (size_t i = 0; i < CraneAmountCurrent; i++)
	{
		if (CraneArray[i] && CraneArray[i]->IsFree()) {
			FVector SpawnVec = GetSpawnVector();

			int32 spawn;
			TSubclassOf<ASItemBase> ItemClass;

			// Set 0 for debug
			if (1) {
				spawn = rand() % 100;

				if (spawn > 25) {
					//Items
					if (spawn < 75) {
						ItemClass = ItemBoxClass;
					}
					else if (spawn < 90) {
						ItemClass = ItemRockClass;
					}
					else if (spawn < 100) {
						ItemClass = ItemExplosiveClass;
					}
				}
				//Boost Items
				else {
					if (spawn < 26) {
						ItemClass = ItemHealClass;
					}
				}
			}
			else {
				spawn = rand() % 1;

				switch (spawn)
				{
				case 0:
					ItemClass = ItemExplosiveClass;
					break;
				case 1:
					ItemClass = ItemRockClass;
					break;
				case 2:
					ItemClass = ItemHealClass;
					break;
				case 3:
					ItemClass = ItemExplosiveClass;
					break;
				}
			}
			FVector destVec = Floor->GetDestVector();
			Floor->SetGridBusy(destVec);

			CraneArray[i]->DeliverItem(destVec, SpawnItem(SpawnVec, ItemClass));
		}
	}
}

//Get a spawn location on the edge of the room
FVector ASRoom::GetSpawnVector()
{
	int32 SpawnVecRand = GridSize * (rand() % (RoomWidth + 2) - 1);
	switch (rand() %3)
	{
	case 0:
		return FVector(-GridSize, SpawnVecRand, SpawnVecHeight);
	case 1:
		return FVector(SpawnVecRand, -GridSize, SpawnVecHeight);
	case 2:
		switch (rand() % 2) {

		case 0:
			return FVector(SpawnVecEdge, SpawnVecRand, SpawnVecHeight);
		case 1:
			return FVector(SpawnVecRand, SpawnVecEdge, SpawnVecHeight);
		}
	default:
		return FVector(-GridSize, -GridSize, GridSize * (StackHeight + 3.5));
	}
}

//Spawn a new item 
ASItemBase *ASRoom::SpawnItem(FVector SpawnVector, TSubclassOf<ASItemBase> ItemClass) {
			
	ASItemBase *item = nullptr;		

	item = GetWorld()->SpawnActor<ASItemBase>(ItemClass, SpawnVector, FRotator::ZeroRotator);

	if (item != nullptr && Floor != nullptr) {
		//Set ptr on the floor actor for calling CheckLines() function after moving
		item->Floor = Floor;
	}

	return item;
}


void ASRoom::CoinSpawner()
{
	SpawnCoin();
	GetWorld()->GetTimerManager().SetTimer(CoinSpawnerHandle, this, &ASRoom::CoinSpawner, rand() % 15, false);
}

ASCoin *ASRoom::SpawnCoin()
{
	ASCoin * coin= GetWorld()->SpawnActor<ASCoin>(CoinClass, Floor->GetDestVector(), FRotator::ZeroRotator);
	if (coin) {
		return coin;
	}
	else {
		return nullptr;
	}
}


const FSaveRoomStruct& ASRoom::SaveRoom()
{
	SaveStruct.RoomWidth = RoomWidth;
	SaveStruct.RoomDifficult = DifficultIncreaseCurrent;

	return SaveStruct;
}

void ASRoom::LoadRoom(const FSaveRoomStruct &loadStruct)
{
	InitializeRoom(loadStruct.RoomWidth);

	for (int32 i = 0; i < loadStruct.RoomDifficult; i++) {
		IncreaseDifficult();
	}
}