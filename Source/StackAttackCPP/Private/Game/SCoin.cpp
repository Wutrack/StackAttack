// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoin.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ASCoin::ASCoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create default objects
	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoinMesh"));
	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));

	RootComponent = CoinMesh;
	CollisionCapsule->AttachToComponent(CoinMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CollisionCapsule->SetGenerateOverlapEvents(true);
	CoinMesh->SetSimulatePhysics(true);

	//Allow Coin move and rotate only in Z axis
	CoinMesh->BodyInstance.bLockXRotation = true;
	CoinMesh->BodyInstance.bLockYRotation = true;
	CoinMesh->BodyInstance.bLockXTranslation = true;
	CoinMesh->BodyInstance.bLockYTranslation = true;
}

// Called when the game starts or when spawned
void ASCoin::BeginPlay()
{
	Super::BeginPlay();

	CoinMesh->SetStaticMesh(CoinStaticMesh);
	CollisionCapsule->SetCapsuleHalfHeight(CapsuleHalfHeight);
	CollisionCapsule->SetCapsuleRadius(CapsuleRadius);
}

// Called every frame
void ASCoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(RotationSpeed);

	//If player collide with coin - its start to rise Up
	if (bCollected) {
		RotationSpeed += FRotator(0, 1, 0);
		AddActorLocalOffset(FVector(0, 0, 2));
	}
}

void ASCoin::CoinCollected()
{
	bCollected = true;
	CollisionCapsule->SetGenerateOverlapEvents(false);

	CoinMesh->SetSimulatePhysics(false);
	CoinMesh->SetCollisionProfileName("IgnoreAll");

	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &AActor::K2_DestroyActor, 1.f, false);
}

//Save-load logic
const FCoinSaveStruct &ASCoin::SaveCoin()
{
	SaveStruct.CoinLocation = GetActorLocation();
	SaveStruct.RotationSpeed = GetActorRotation();
	SaveStruct.bCollected = bCollected;
	SaveStruct.CoinValue = CoinValue;

	return SaveStruct;
}

void ASCoin::LoadCoin(const FCoinSaveStruct &Save)
{
	bCollected = Save.bCollected;
	CoinValue = Save.CoinValue;
}

