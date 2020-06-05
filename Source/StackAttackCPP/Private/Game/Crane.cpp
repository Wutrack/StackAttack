// Fill out your copyright notice in the Description page of Project Settings.


#include "Crane.h"
#include "Item.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/StaticMeshSocket.h"
#include "Components/StaticMeshComponent.h"
#include <Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h>

// Sets default values
ACrane::ACrane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ResetSpeed();
	bFree = true;
	bAttached = false;

	UStaticMesh *Cone = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/Mesh/myCone")).Object;
	UStaticMesh *Cube = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cube")).Object;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	CraneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crane"));
	BeamMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Beam"));

	RootComponent = RootScene;

	CraneMesh->SetStaticMesh(Cone);
	BeamMesh->SetStaticMesh(Cube);

	CraneMesh->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
	BeamMesh->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
}

void ACrane::InitCrane(const FVector startPos, const int32 roomWidth, const int32 gridSize)
{
	RootScene->SetWorldLocation(FVector(0, 0, startPos.Z));

	BeamMesh->SetWorldScale3D(FVector(1, roomWidth + 4, 1));
	BeamMesh->SetRelativeLocation(FVector(startPos.X, roomWidth / 2 * gridSize, 50));

	CraneMesh->SetRelativeLocation(FVector(startPos.X, startPos.Y, -100));
}

void ACrane::DeliverItem(const FVector destLoc, AItem *itemArg)
{
	if (itemArg) {
		bFree = false;
		Item = itemArg;
		MoveToLoc = FVector(Item->GetActorLocation());
		DetachLoc = destLoc; 
		GetWorld()->GetTimerManager().SetTimer(MoveCraneHandle, this, &ACrane::MoveCrane, 0.01, true, 0);
	}
}

void ACrane::HalfWay(AItem* item)
{
	Item = item;
	AttachItem();
	GetWorld()->GetTimerManager().SetTimer(MoveCraneHandle, this, &ACrane::MoveCrane, 0.01, true, 0);
}

void ACrane::ResetSpeed()
{
	CraneSpeed = CraneSpeedStart;
}

void ACrane::IncreaseSpeed(int32 increase)
{
	CraneSpeed += increase;
}


void ACrane::MoveCrane()
{
	float DeltaTime = GetWorld()->DeltaTimeSeconds;

	CraneMesh->SetRelativeLocation(FVector(
		FMath::FInterpConstantTo(CraneMesh->GetRelativeLocation().X, MoveToLoc.X, DeltaTime, CraneSpeed),
		FMath::FInterpConstantTo(CraneMesh->GetRelativeLocation().Y, MoveToLoc.Y, DeltaTime, CraneSpeed),
		CraneMesh->GetRelativeLocation().Z));
	BeamMesh->SetRelativeLocation(FVector(
		FMath::FInterpConstantTo(BeamMesh->GetRelativeLocation().X, MoveToLoc.X, DeltaTime, CraneSpeed),
		BeamMesh->GetRelativeLocation().Y, BeamMesh->GetRelativeLocation().Z));

	if (CraneMesh->GetRelativeLocation().X == MoveToLoc.X && CraneMesh->GetRelativeLocation().Y == MoveToLoc.Y) {
		if (bAttached) {
			DetachItem();
		}
		else
		{
			AttachItem();
			MoveToLoc = DetachLoc;
		}
	}		
}

void ACrane::AttachItem()
{
	bAttached = true;
	Item->AttachToComponent(CraneMesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), "Item");
	Item->OnAttach();
}

void ACrane::DetachItem()
{
	Item->OnDetach();
	Item = nullptr;
	bAttached = false;
	bFree = true;
	GetWorld()->GetTimerManager().ClearTimer(MoveCraneHandle);
}

FSaveCraneStruct ACrane::SaveCrane()
{
	FSaveCraneStruct save;

	save.bAttached = bAttached;
	save.bFree = bFree;
	save.CraneSpeed = CraneSpeed;
	save.DetachLoc = DetachLoc;
	save.MoveToLoc = MoveToLoc;

	save.RelLocBeam = BeamMesh->GetRelativeLocation();
	save.RelLocCrane = CraneMesh->GetRelativeLocation();

	return save;
}

void ACrane::LoadCrane(FSaveCraneStruct load)
{
	bAttached = load.bAttached;
	bFree = load.bFree;
	CraneSpeed = load.CraneSpeed;
	DetachLoc = load.DetachLoc;
	MoveToLoc = load.MoveToLoc;

	BeamMesh->SetRelativeLocation(load.RelLocBeam);
	CraneMesh->SetRelativeLocation(load.RelLocCrane);

}
// Called when the game starts or when spawned
void ACrane::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACrane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

