// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SItemBase.generated.h"

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
	FVector					ItemLocation;
	UPROPERTY(EditAnywhere)
	EItemType				ItemType;
	UPROPERTY(EditAnywhere)
	EBoostType				BoostType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASItemBase> ItemClass;

	UPROPERTY(EditAnywhere)
	bool					bJustSpawned;
	UPROPERTY(EditAnywhere)
	bool					bAttachedToCrane;
	UPROPERTY(EditAnywhere)
	bool					bDormancy;

	UPROPERTY(EditAnywhere)
	bool					bFalling;
	UPROPERTY(EditAnywhere)
	float					FallSpeedCurrent;

	UPROPERTY(EditAnywhere)
	bool					bPushing;
	UPROPERTY(EditAnywhere)
	FVector					PushDestination;

	UPROPERTY(EditAnywhere)
	TArray<UMaterial*>		MaterialCollection;

	UPROPERTY(EditAnywhere)
	TArray<bool>			AdditionalBoolValue;
	UPROPERTY(EditAnywhere)
	TArray<float>			AdditionalFloatValue;


	void NewElement(FVector Location, EItemType itemType, EBoostType boostType, TSubclassOf<ASItemBase> itemClass) {
		ItemLocation		= Location;
		ItemType			= itemType;
		BoostType			= boostType;
		ItemClass			= itemClass;
	}

	void SaveFalling(bool falling, float fallSpeed) {
		bFalling			= falling;
		FallSpeedCurrent	= FallSpeedCurrent;
	}

	void SaveMoving(bool pushing, FVector pushDestination) {
		bPushing			= pushing;
		PushDestination		= pushDestination;
	}

	void SaveBool(bool attachedToCrane, bool justSpawned, bool dormancy) {
		bAttachedToCrane	= attachedToCrane;
		bJustSpawned		= justSpawned;
		bDormancy			= dormancy;
	}

	FSaveItemStruct() {
		ItemLocation		= FVector();
		ItemType			= EItemType::Box;
		BoostType			= EBoostType::None;

		bJustSpawned		= false;
		bAttachedToCrane	= false;
		bFalling			= false;
		bDormancy			= false;
		bPushing			= false;

		FallSpeedCurrent	= 0;
		PushDestination		= FVector();
	}
};

UCLASS()
class STACKATTACKCPP_API ASItemBase : public AActor
{
	GENERATED_BODY()

private:

	//Actor on what item is landed. Using for OnEndOverlap event
	AActor			*BottomItem			= nullptr;

	//Item states
	bool			bJustSpawned		= false,		
					bAttachedToCrane	= false, 
					bDormancy			= false,
					bFalling			= false,
					bPushing			= false;

	//Falling parameters
	FTimerHandle	FallingTHandle;
	float			FallSpeedStart		= 1, 
					FallSpeedCurrent	= 1, 
					FallSpeedBoost		= 1.02;

	//Push parameters
	FTimerHandle	MoveTHandle;
	FVector			TraceStart, 
					TraceEnd, 
					MoveDestVec;
	FQuat			Fquat;
	float			PushSpeed		= 500;

protected:
	//Components
	UStaticMeshComponent *ItemMesh = nullptr;
	UMaterial *ItemMaterial = nullptr;
	class UBoxComponent *CollisionItemBox = nullptr;

	//Body parts collection for random creation
	UPROPERTY(EditAnywhere)
	TArray<UMaterial *>			MaterialCollection;
	UPROPERTY(EditAnywhere)	
	TArray<UMaterial *>			SpecialMaterialCollection;
	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh *>		MeshCollection;

	//Sound 
	UPROPERTY(EditAnywhere)
	UAudioComponent				*AudioComponent		= nullptr;
	UPROPERTY(EditAnywhere)
	USoundBase					*PushSound			= nullptr;
	UPROPERTY(EditAnywhere)
	USoundBase					*LandedSound		= nullptr;
	UPROPERTY(EditAnywhere)
	USoundBase					*DestroyedSound		= nullptr;

	//Protected item parameters
	EItemType			ItemType;
	EBoostType			BoostType		= EBoostType::None;

	bool				bCanPickUpItem	= false;
	int32				BoostPower		= 1,
						Points			= 10,
						HitDamage		= 1;

	//Movig logic
	void			PushItem();
	virtual void	StopPush();
	void			StartFalling();
	void			Falling();
	virtual void	StopFalling();

	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

public:	
	// Sets default values for this actor's properties
	ASItemBase();

	class ASFloor *Floor;

	//Get Item state functions
	bool	GetJustSpawned() const			{ return bJustSpawned; };
	bool	GetAttachedToCrane() const		{ return bAttachedToCrane; };
	bool	GetPushing() const				{ return bPushing; };
	bool	GetFalling() const				{ return bFalling; };
	bool	GetDormancy() const				{ return bDormancy; };

	void	OnAttachToCrane();
	void	OnDetachFromCrane();

	UStaticMeshComponent *GetMesh() const		{ return ItemMesh; };

	EItemType		GetType() const				{ return ItemType; };
	EBoostType		GetBoostType() const		{ return BoostType; };
	int32			GetPoints() const			{ return Points; };
	int32			GetBoostPower() const		{ return BoostPower; };
	bool			CanTake() const				{ return bCanPickUpItem; };

	//Check if someting prevent ahead
	virtual bool	CanPush			(const FVector);
	void			Push			(const FVector);

	UFUNCTION()
	virtual void	DestroyItem();

	//Save-Load functions
	virtual FSaveItemStruct		SaveItem();
	virtual ASItemBase*			LoadItem		(const FSaveItemStruct&);


	FTimerHandle TestTHandle;
	void UpdateOverlapAfterLoading();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
