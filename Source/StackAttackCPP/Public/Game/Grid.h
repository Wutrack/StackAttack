// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"


enum class gridState {
	HasSpace,
	Busy,
	FULL
};

/**
 * 
 */
UCLASS()
class STACKATTACKCPP_API AGrid : public AActor
{
	GENERATED_BODY()

	class UBoxComponent *box;
	class AItem *item;
	class AMarker *marker;

	int blockCount, blockCountMax;
	gridState state;

	void setState();
	void SpawnMarker();
public:
	AGrid();
	~AGrid();


	void initGrid(const float);

	gridState getState() { return state; }

	bool canSpawn();
	void setBusy();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

