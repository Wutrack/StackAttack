// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Crane.generated.h"

USTRUCT()
struct FSaveCraneStruct {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
		FVector RelLocBeam;
	UPROPERTY(EditAnywhere)
		FVector RelLocCrane;

	UPROPERTY(EditAnywhere)
		FVector MoveToLoc;
	UPROPERTY(EditAnywhere)
		FVector DetachLoc;

	UPROPERTY(EditAnywhere)
		bool bFree;
	UPROPERTY(EditAnywhere)
		bool bAttached;

	UPROPERTY(EditAnywhere)
		float CraneSpeed;
};

UCLASS()
class STACKATTACKCPP_API ACrane : public AActor
{
	GENERATED_BODY()
private:

	UStaticMeshComponent *CraneMesh, *BeamMesh;
	class UMaterialInstance *CraneMaterial, *BeamMaterial;
	USceneComponent *RootScene;
	class AItem *Item;

	FTimerHandle MoveCraneHandle;
	FVector MoveToLoc, DetachLoc;

	bool bFree, bAttached;
	const float CraneSpeedStart = 400;
	float CraneSpeed;

	void AttachItem();
	void MoveCrane();
	void DetachItem();

public:	
	// Sets default values for this actor's properties
	ACrane();
	void InitCrane(const FVector, const int32, const int32);
	bool IsFree() const { return bFree; };
	bool IsAttached() const { return bAttached; };
	void DeliverItem(const FVector, AItem*);

	void HalfWay(class AItem*);

	void ResetSpeed();
	void IncreaseSpeed(int32 increase = 50);

	FSaveCraneStruct SaveCrane();
	void LoadCrane(FSaveCraneStruct);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
