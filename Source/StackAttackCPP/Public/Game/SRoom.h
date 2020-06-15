// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRoom.generated.h"

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
class STACKATTACKCPP_API ASRoom : public AActor
{

	GENERATED_BODY()

private:

	//BP spawn classes
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASItemBase>	ItemBoxClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASItemBase>	ItemRockClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASItemBase>	ItemHealClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASItemBase>	ItemExplosiveClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASCoin>		CoinClass;

	//Room parameters
	const int32						GridSize			= 100, 
									StackHeight			= 7;
	int32							RoomWidth			= 10;

	//Crane parameters
	int32							CraneAmountMax, 
									CraneAmountCurrent	= 0;
	TArray<class ASCrane *>			CraneArray;
	int32							SpawnVecEdge, 
									SpawnVecHeight;
	 
	FVector			GetSpawnVector();
	void			SearchFreeCraneAndSpawnItem();

	//Difficult
	void			IncreaseDifficult();
	int32			DifficultIncreaseCurrent = 0, 
					DifficultIncreaseMax;
	FTimerHandle	DifficultIncreaseHandle;

	FSaveRoomStruct SaveStruct;
	class ASFloor	*Floor;
	FTimerHandle	CraneSearchTHandle;

public:	
	// Sets default values for this actor's properties
	ASRoom();

	class ASItemBase*		SpawnItem			(FVector, TSubclassOf<class ASItemBase>);
	ASCrane*				SpawnNewCrane();
	
	FTimerHandle			CoinSpawnerHandle;
	void					CoinSpawner();
	class ASCoin *			SpawnCoin();

	//Initialize room parameters
	void					InitializeRoom		(int32);

	int32					GetDifficultIncreaseCount() const	{ return DifficultIncreaseCurrent; };
	int32					GetRoomwidth() const				{ return RoomWidth; };
	ASFloor *				GetFloor() const					{ return Floor; };

	//Save-Load logic
	const FSaveRoomStruct&	SaveRoom();
	void					LoadRoom				(const FSaveRoomStruct &);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime);

};

