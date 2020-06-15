// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCrane.generated.h"

USTRUCT()
struct FSaveCraneStruct {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
		FVector					BeamLocation;
	UPROPERTY(EditAnywhere)
		FVector					CraneLocation;

	UPROPERTY(EditAnywhere)
		FVector					LocationMoveTo;
	UPROPERTY(EditAnywhere)
		FVector					LocationToDetachItem;

	UPROPERTY(EditAnywhere)
		bool					bFree;
	UPROPERTY(EditAnywhere)
		bool					bHasAttachedItem;

	UPROPERTY(EditAnywhere)
		float					CraneSpeedCurrent;
};

UCLASS()
class STACKATTACKCPP_API ASCrane : public AActor
{
	GENERATED_BODY()
private:

	UStaticMeshComponent	*CraneMesh		= nullptr, 
							*BeamMesh		= nullptr;

	UMaterial				*CraneMaterial	= nullptr,
							*BeamMaterial	= nullptr;

	USceneComponent			*RootScene		= nullptr;
	class ASItemBase		*DeliveringItem = nullptr;

	FTimerHandle			MoveCraneHandle;
	FVector					LocationMoveTo,
							LocationToDetachItem;

	bool					bFree				= true, 
							bHasAttachedItem	= false;
	const float				CraneSpeedStart		= 400;
	float					CraneSpeedCurrent   = 400;

	void					AttachItem();
	void					MoveCrane();
	void					DetachItem();

public:	
	// Sets default values for this actor's properties
	ASCrane();

	//Generate crane
	void	InitCrane		(const FVector&, const int32&, const int32&);
	//Deliver Item from spawned location
	void	DeliverItem		(const FVector, ASItemBase*);
	//Increase speed on increase difficult
	void	IncreaseSpeed	(int32 increase = 50);
	void	ResetSpeed();
	
	bool	IsFree() const					{ return bFree; };
	bool	IsHasAttachedItem() const		{ return bHasAttachedItem; };

	//Need to change it
	void	HalfWay			(class ASItemBase *);

protected:
	// Called when the game starts or when spawned
	virtual void	BeginPlay() override;

public:	
	// Called every frame
	virtual void	Tick	(float DeltaTime) override;

	//Save-Load logic
	FSaveCraneStruct	SaveCrane();
	void				LoadCrane			(FSaveCraneStruct);
};
