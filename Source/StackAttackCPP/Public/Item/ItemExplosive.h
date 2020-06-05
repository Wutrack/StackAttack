// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemExplosive.generated.h"

/**
 * 
 */
UCLASS()
class STACKATTACKCPP_API AItemExplosive : public AItem
{
	GENERATED_BODY()

	UMaterial *ActiveMaterial;
	class USphereComponent *SphereColl;

	FTimerHandle ExplodeHandle;

	//Bool states 
	bool bExplode, bActive, bExplodeActive;


	//Must to roll active on push and falling
	virtual void StopPush() override;
	virtual void StopFalling() override;

	void RollToActive();

	//Normal explosion
	void SetActive();
	//Quick explosion
	void SetExplodeActive();
	//Common active change
	void SetActiveChange();


	void Explode();
public:
	AItemExplosive();

	virtual void DestroyItem() override;
	virtual void BeginPlay() override;

	virtual FSaveItemStruct SaveItem() override;
	virtual AItem *LoadItem(const FSaveItemStruct&) override;
};
