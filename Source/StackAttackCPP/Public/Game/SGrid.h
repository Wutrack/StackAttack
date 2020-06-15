// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGrid.generated.h"


enum class gridState {
	HasSpace,
	Busy,
	FULL
};

/**
 * 
 */
UCLASS()
class STACKATTACKCPP_API ASGrid : public AActor
{
	GENERATED_BODY()

	class UBoxComponent		*CollisionBox		= nullptr;
	class ASItemBase		*IncomingItem		= nullptr;
	class ASMarker			*Marker				= nullptr;

	int						ItemCountCurrent	= 0, 
							ItemCountMax		= 0;
	gridState				GridState			= gridState::HasSpace;

	void		SetState();
	void		SpawnMarker();
public:
	ASGrid();
	~ASGrid();

	void		InitializeGrid		(const float);

	gridState	GetGridState()		{ return GridState; }

	bool		CanSpawn();
	void		SetBusy();

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

