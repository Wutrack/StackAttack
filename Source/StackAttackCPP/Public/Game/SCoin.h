// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCoin.generated.h"

USTRUCT()
struct FCoinSaveStruct {
	GENERATED_USTRUCT_BODY()

	FVector		CoinLocation; 
	FRotator	RotationSpeed;
	bool		bCollected;
	int32		CoinValue;
};


UCLASS()
class STACKATTACKCPP_API ASCoin : public AActor
{
	GENERATED_BODY()

	//Save struct
	FCoinSaveStruct SaveStruct;

	//Coin`s components
	UStaticMeshComponent			*CoinMesh			= nullptr;
	class UCapsuleComponent			*CollisionCapsule	= nullptr;

	UPROPERTY(EditAnywhere)
	UStaticMesh						*CoinStaticMesh		= nullptr;

	//Capsule size
	UPROPERTY(EditAnywhere)
	float							CapsuleHalfHeight	= 30;
	UPROPERTY(EditAnywhere)
	float							CapsuleRadius		= 20;

	bool							bCollected			= false;
	FRotator						RotationSpeed		= FRotator(0, 1, 0);

public:	
	// Sets default values for this actor's properties
	ASCoin();

	int32		CoinValue		= 1;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CoinCollected();

	//Save-load logic
	const FCoinSaveStruct&		SaveCoin();
	void						LoadCoin(const FCoinSaveStruct&);
};
