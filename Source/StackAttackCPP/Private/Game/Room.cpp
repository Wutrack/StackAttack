// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"
#include "ItemList.h"
#include "Crane.h"
#include "Floor.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "UObject/ConstructorHelpers.h"


ARoom::ARoom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DifficultIncreaseCount = 0;
	CraneAmount = 0;
}

void ARoom::InitRoom(int32 roomWidth)
{
	RoomWidth = roomWidth;

	SpawnVecEdge = GridSize * (RoomWidth);
	SpawnVecZ = GridSize * (StackHeight + 3.5);

	CraneAmountMax = RoomWidth / 3;

	DifficultMax = CraneAmountMax * 5;

	//Create a floor actor
	Floor = GetWorld()->SpawnActor<AFloor>(AFloor::StaticClass(), FVector(0), FRotator::ZeroRotator);
	if (Floor) {
		Floor->InitFloor(RoomWidth, GridSize, StackHeight);
	}

	//Start searching a free crane timer
	GetWorld()->GetTimerManager().SetTimer(CraneSearchTHandle, this, &ARoom::SearchFreeCraneAndSpawnItem, 1.0f, true, 2.0f);

	//Start increase the level difficult timer
	GetWorld()->GetTimerManager().SetTimer(DifficultHandle, this, &ARoom::IncreaseDifficult, 30.0f, true, 0);	
}


// Called when the game starts or when spawned
void ARoom::BeginPlay()
{	

}

// Called every frame
void ARoom::Tick(float DeltaTime)
{

}

//Increase the difficult periodically
void ARoom::IncreaseDifficult()
{
	if (DifficultIncreaseCount != DifficultMax) {
		if (DifficultIncreaseCount % 5 == 0 && CraneAmount != CraneAmountMax) {
			SpawnNewCrane();
			for (size_t i = 0; i < CraneAmount - 1; i++)
			{
				CraneArr[i]->ResetSpeed();
			}
		}
		else
		{
			for (size_t i = 0; i < CraneAmount; i++)
			{
				CraneArr[i]->IncreaseSpeed();
			}
		}
	}
	else {
		//If the level difficult is max - clear timer
		GetWorld()->GetTimerManager().ClearTimer(DifficultHandle);
	}
	DifficultIncreaseCount++;
}


ACrane* ARoom::SpawnNewCrane()
{
	if (CraneAmount != CraneAmountMax) {
		ACrane *crane = GetWorld()->SpawnActor<ACrane>(ACrane::StaticClass(), FVector(0, 0, (StackHeight + 3) * GridSize),
			FRotator::ZeroRotator);
		if (crane) {
			CraneArr.Add(crane);
			crane->InitCrane(FVector(150, 350, (StackHeight + 5) * GridSize), RoomWidth, GridSize);
			CraneAmount++;
			return crane;
		}
	}
	return nullptr;
}


//Search for a free crane. If found - spawn a new item and deliver it
void ARoom::SearchFreeCraneAndSpawnItem()
{
	for (size_t i = 0; i < CraneAmount; i++)
	{
		if (CraneArr[i] && CraneArr[i]->IsFree()) {
			FVector SpawnVec = GetSpawnVector();

			int32 spawn;
			TSubclassOf<AItem> ItemClass;

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
			CraneArr[i]->DeliverItem(Floor->GetDestVector(), SpawnItem(SpawnVec, ItemClass));
		}
	}
}

//Get a spawn location on the edge of the room
FVector ARoom::GetSpawnVector()
{
	SpawnVecRand = GridSize * (rand() % (RoomWidth + 2) - 1);
	switch (rand() %3)
	{
	case 0:
		return FVector(-GridSize, SpawnVecRand, SpawnVecZ);
	case 1:
		return FVector(SpawnVecRand, -GridSize, SpawnVecZ);
	case 2:
		switch (rand() % 2) {

		case 0:
			return FVector(SpawnVecEdge, SpawnVecRand, SpawnVecZ);
		case 1:
			return FVector(SpawnVecRand, SpawnVecEdge, SpawnVecZ);
		}
	default:
		return FVector(-GridSize, -GridSize, GridSize * (StackHeight + 3.5));
	}
}

//Spawn a new item 
AItem *ARoom::SpawnItem(FVector SpawnVector, TSubclassOf<AItem> ItemClass) {
			
	AItem *item = nullptr;		

	item = GetWorld()->SpawnActor<AItem>(ItemClass, SpawnVector, FRotator::ZeroRotator);

	if (item != nullptr && Floor != nullptr) {
		//Set ptr on the floor actor for calling CheckLines() function after moving
		item->Floor = Floor;
	}

	return item;
}



const FSaveRoomStruct& ARoom::SaveRoom()
{
	SaveStruct.RoomWidth = RoomWidth;
	SaveStruct.RoomDifficult = DifficultIncreaseCount;

	return SaveStruct;
}

void ARoom::LoadRoom(const FSaveRoomStruct &loadStruct)
{
	InitRoom(loadStruct.RoomWidth);

	for (int32 i = 0; i < loadStruct.RoomDifficult; i++) {
		IncreaseDifficult();
	}
}