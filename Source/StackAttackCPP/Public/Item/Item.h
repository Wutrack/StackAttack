// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM()
enum class EItemType {
	Box,
	Boost,
	Explosive,
	Rock,
};

UENUM()
enum class EBoostType {
	None,
	Heal, 
	IncMaxHealth,
	Shield,
};

USTRUCT()
struct FSaveItemStruct {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
		FVector ItemLocation;
	UPROPERTY(EditAnywhere)
		EItemType ItemType;
	UPROPERTY(EditAnywhere)
		EBoostType BoostType;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AItem> ItemClass;

	UPROPERTY(EditAnywhere)
		bool bFresh;
	UPROPERTY(EditAnywhere)
		bool bAttached;
	UPROPERTY(EditAnywhere)
		bool bLanded;

	UPROPERTY(EditAnywhere)
		bool bFalling;
	UPROPERTY(EditAnywhere)
		float FallSpeed;

	UPROPERTY(EditAnywhere)
		bool bMoving;
	UPROPERTY(EditAnywhere)
		FVector MoveDestVec;

	UPROPERTY(EditAnywhere)
		TArray<UMaterial*> MatArr;

	UPROPERTY(EditAnywhere)
		TArray<bool> AdditionalBool;
	UPROPERTY(EditAnywhere)
		TArray<float> AdditionalFloat;


	void NewElement(FVector Location, EItemType itemType, EBoostType boostType, TSubclassOf<AItem> itemClass) {
		ItemLocation = Location;
		ItemType = itemType;
		BoostType = boostType;
		ItemClass = itemClass;
	}
	void SaveFalling(bool falling, float fallSpeed) {
		bFalling = falling;
		FallSpeed = fallSpeed;
	}
	void SaveMoving(bool moving, FVector moveDestVec) {
		bMoving = moving;
		MoveDestVec = moveDestVec;
	}

	void SaveBool(bool attached, bool fresh, bool landed) {
		bAttached = attached;
		bFresh = fresh;
		bLanded = landed;
	}

	FSaveItemStruct() {
		ItemLocation = FVector();
		ItemType = EItemType::Box;
		BoostType = EBoostType::None;

		bFresh = false;
		bAttached = false;
		bFalling = false;
		bLanded = false;
		bMoving = false;

		FallSpeed = 0;
		MoveDestVec = FVector();
	}
};

UCLASS()
class STACKATTACKCPP_API AItem : public AActor
{
	GENERATED_BODY()

	AActor *BottomItem;

	bool bFresh, bAttached, bLanded;

	//Falling parameters
	FTimerHandle FallingTHandle;
	bool bFalling;
	float FallSpeedStart, FallSpeed, FallBoost;

	//Move parameters
	FTimerHandle MoveTHandle;
	FVector TraceStart, TraceEnd, MoveDestVec;
	FQuat Fquat;
	bool bMoving;
	float MoveSpeed;

protected:

	UPROPERTY(EditAnywhere)
		TArray<UMaterial *> MaterialArrBP;
	UPROPERTY(EditAnywhere)
		TArray<UMaterial *> MaterialArrSpecialBP;
	UPROPERTY(EditAnywhere)
		TArray<UStaticMesh *> MeshArrBP;

	EItemType Type;
	EBoostType BoostType;
	bool bTake;
	int32 BoostPower;
	int32 Points;

	int32 HitDamage;

	//Components
	UStaticMeshComponent *ItemMesh;
	UMaterial *ItemMaterialInst;
	class UBoxComponent *ItemBox;

	void PushItem();
	virtual void StopPush();
	void StartFalling();
	virtual void StopFalling();
	void Falling();

	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

public:	
	// Sets default values for this actor's properties
	AItem();

	class AFloor *Floor;

	bool GetFresh() const { return bFresh; };
	bool GetAttached() const { return bAttached; };
	bool GetMoving() const { return bMoving; };
	bool GetFalling() const { return bFalling; };
	bool GetLanded() const { return bLanded; };

	void OnAttach();
	void OnDetach();

	UStaticMeshComponent *GetMesh() const { return ItemMesh; };

	EItemType GetType() const { return Type; };
	EBoostType GetBoostType() const { return BoostType; };
	int32 GetPoints() const { return Points; };
	int32 GetBoostPower() const { return BoostPower; };
	bool CanTake() const { return bTake; };
	virtual bool CanPush(const FVector);
	void Push(const FVector);

	UFUNCTION()
	virtual void DestroyItem();

	virtual FSaveItemStruct SaveItem();
	virtual AItem* LoadItem(const FSaveItemStruct&);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
