// Fill out your copyright notice in the Description page of Project Settings.


#include "SCrane.h"
#include "SItemBase.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/StaticMeshSocket.h"
#include "Components/StaticMeshComponent.h"
#include <Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h>

// Sets default values
ASCrane::ASCrane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	CraneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CraneMesh"));
	BeamMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BeamMesh"));

	RootComponent = RootScene;

	//Get meshes for crane
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cone (TEXT("/Game/Mesh/myCone"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Beam (TEXT("/Engine/BasicShapes/Cube"));

	if (Beam.Succeeded()) {
		BeamMesh->SetStaticMesh(Beam.Object);
	}
	if (Cone.Succeeded()) {
		CraneMesh->SetStaticMesh(Cone.Object);
	}

	CraneMesh->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
	BeamMesh->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
}

void ASCrane::InitCrane(const FVector &startPos, const int32 &roomWidth, const int32 &gridSize)
{
	RootScene->SetWorldLocation(FVector(0, 0, startPos.Z));

	BeamMesh->SetWorldScale3D(FVector(1, roomWidth + 4, 1));
	BeamMesh->SetRelativeLocation(FVector(startPos.X, roomWidth / 2 * gridSize, 50));

	CraneMesh->SetRelativeLocation(FVector(startPos.X, startPos.Y, -100));
}

void ASCrane::DeliverItem(const FVector locationToDetachItem, ASItemBase *ItemToDeliver)
{
	if (ItemToDeliver) {
		bFree = false;
		DeliveringItem = ItemToDeliver;
		LocationMoveTo = FVector(DeliveringItem->GetActorLocation());
		LocationToDetachItem = locationToDetachItem;
		GetWorld()->GetTimerManager().SetTimer(MoveCraneHandle, this, &ASCrane::MoveCrane, 0.01, true, 0);
	}
}

void ASCrane::HalfWay(ASItemBase* item)
{
	DeliveringItem = item;
	AttachItem();
	GetWorld()->GetTimerManager().SetTimer(MoveCraneHandle, this, &ASCrane::MoveCrane, 0.01, true, 0);
}

void ASCrane::ResetSpeed()
{
	CraneSpeedCurrent = CraneSpeedStart;
}

void ASCrane::IncreaseSpeed(int32 increase)
{
	CraneSpeedCurrent += increase;
}


void ASCrane::MoveCrane()
{
	float DeltaTime = GetWorld()->DeltaTimeSeconds;

	CraneMesh->SetRelativeLocation(FVector(
		FMath::FInterpConstantTo(CraneMesh->GetRelativeLocation().X, LocationMoveTo.X, DeltaTime, CraneSpeedCurrent),
		FMath::FInterpConstantTo(CraneMesh->GetRelativeLocation().Y, LocationMoveTo.Y, DeltaTime, CraneSpeedCurrent),
		CraneMesh->GetRelativeLocation().Z));
	BeamMesh->SetRelativeLocation(FVector(
		FMath::FInterpConstantTo(BeamMesh->GetRelativeLocation().X, LocationMoveTo.X, DeltaTime, CraneSpeedCurrent),
		BeamMesh->GetRelativeLocation().Y, BeamMesh->GetRelativeLocation().Z));

	if (CraneMesh->GetRelativeLocation().X == LocationMoveTo.X && CraneMesh->GetRelativeLocation().Y == LocationMoveTo.Y) {
		if (bHasAttachedItem) {
			DetachItem();
		}
		else
		{
			AttachItem();
			LocationMoveTo = LocationToDetachItem;
		}
	}		
}

void ASCrane::AttachItem()
{
	bHasAttachedItem = true;
	DeliveringItem->AttachToComponent(CraneMesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), "Item");
	DeliveringItem->OnAttachToCrane();
}

void ASCrane::DetachItem()
{
	DeliveringItem->OnDetachFromCrane();
	DeliveringItem = nullptr;
	bHasAttachedItem = false;
	bFree = true;
	GetWorld()->GetTimerManager().ClearTimer(MoveCraneHandle);
}

FSaveCraneStruct ASCrane::SaveCrane()
{
	FSaveCraneStruct save;

	save.bHasAttachedItem = bHasAttachedItem;
	save.bFree = bFree;
	save.CraneSpeedCurrent = CraneSpeedCurrent;
	save.LocationToDetachItem = LocationToDetachItem;
	save.LocationMoveTo = LocationMoveTo;

	save.BeamLocation = BeamMesh->GetRelativeLocation();
	save.CraneLocation = CraneMesh->GetRelativeLocation();

	return save;
}

void ASCrane::LoadCrane(FSaveCraneStruct load)
{
	bHasAttachedItem = load.bHasAttachedItem;
	bFree = load.bFree;
	CraneSpeedCurrent = load.CraneSpeedCurrent;
	LocationToDetachItem = load.LocationToDetachItem;
	LocationMoveTo = load.LocationMoveTo;

	BeamMesh->SetRelativeLocation(load.BeamLocation);
	CraneMesh->SetRelativeLocation(load.CraneLocation);

}
// Called when the game starts or when spawned
void ASCrane::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASCrane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

