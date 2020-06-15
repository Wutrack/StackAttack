// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SItemBase.h"
#include "SItemExplosive.generated.h"

/**
 * 
 */
UCLASS()
class STACKATTACKCPP_API ASItemExplosive : public ASItemBase
{
	GENERATED_BODY()

private:
	UMaterial					*ActiveMaterial;
	class USphereComponent		*CollisionSphere;

	FTimerHandle				ExplodeHandle;

	//Bool states 
	bool		bExplode		= false, 
				bNormalActive			= false, 
				bExplodeActive	= false;


	//Must to roll active on push and falling
	virtual void	StopPush() override;
	virtual void	StopFalling() override;

	//Roll to active after moving
	void			RollToActive();

	//Normal explosion
	void			SetNormalActiveState();
	//Quick explosion
	void			SetExplodeActiveState();
	//Common active change
	void			SetActiveChange();
	//Explode logic
	void			Explode();
public:
	ASItemExplosive();

	virtual void	DestroyItem() override;
	virtual void	BeginPlay() override;

	//Save-load functions
	virtual FSaveItemStruct			SaveItem() override;
	virtual ASItemBase				*LoadItem(const FSaveItemStruct&) override;
};
