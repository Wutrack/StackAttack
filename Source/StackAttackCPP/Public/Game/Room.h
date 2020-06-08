// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

USTRUCT()
struct FSaveRoomStruct {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
		int32 RoomWidth;
	UPROPERTY(EditAnywhere)
		int32 RoomDifficult;

	FSaveRoomStruct() {
		RoomWidth = 10;
		RoomDifficult = 0;
	}
};

UCLASS()
class STACKATTACKCPP_API ARoom : public AActor
{

	GENERATED_BODY()

	class AFloor *Floor;
	FTimerHandle CraneSearchTHandle;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AItem> ItemBoxClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AItem> ItemRockClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AItem> ItemHealClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AItem> ItemExplosiveClass;

	//Room parameters
	const int32 GridSize = 100, StackHeight = 7;
	int32 RoomWidth;

	//Crane parameters
	int32 CraneAmountMax, CraneAmount;
	TArray<class ACrane *> CraneArr;
	int32 SpawnVecRand, SpawnVecEdge, SpawnVecZ;
	 
	FVector GetSpawnVector();
	void SearchFreeCraneAndSpawnItem();

	//Difficult
	void IncreaseDifficult();
	int32 DifficultIncreaseCount, DifficultMax;
	FTimerHandle DifficultHandle;

	FSaveRoomStruct SaveStruct;

public:	
	// Sets default values for this actor's properties
	ARoom();

	class AItem *SpawnItem(FVector, TSubclassOf<class AItem>);
	ACrane* SpawnNewCrane();
	

	//Initialize room parameters
	void InitRoom(int32);
	int32 GetRoomwidth() const { return RoomWidth; };
	AFloor *GetFloor() const { return Floor; };

	int32 GetDifficultIncreaseCount() const { return DifficultIncreaseCount; };


	const FSaveRoomStruct& SaveRoom();
	void LoadRoom(const FSaveRoomStruct &);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime);

};

