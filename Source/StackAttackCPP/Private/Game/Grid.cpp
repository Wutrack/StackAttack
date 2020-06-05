// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Item.h"
#include "Marker.h"
#include "Engine/Engine.h"
#include "Components/BoxComponent.h"

void AGrid::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) &&
		Cast<AItem>(OtherActor))
	{
		if (Cast<AItem>(OtherActor)->GetFalling()) {
			item = Cast<AItem>(OtherActor);
			if (marker == nullptr) {
				SpawnMarker();
			}
			if (marker != nullptr) {
				marker->changeMat();
			}
		}
		else {
			if (marker != nullptr)marker->AddActorLocalOffset(FVector(0, 0, 100));
		}
		setState();
		blockCount++;
	}
}

void AGrid::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) 
		&& Cast<AItem>(OtherActor))
	{
		if (IsValid(marker))
			marker->AddActorLocalOffset(FVector(0, 0, -100));
		blockCount--;		
		setState();
	}
}

AGrid::AGrid()
{
	PrimaryActorTick.bCanEverTick = true;

	blockCount = 0;
	state = gridState::HasSpace;

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollGrid"));

	RootComponent = box;

	box->OnComponentBeginOverlap.AddDynamic(this, &AGrid::OnOverlapBegin);
	box->OnComponentEndOverlap.AddDynamic(this, &AGrid::OnOverlapEnd);

	box->SetCollisionProfileName("Grid");
}

AGrid::~AGrid()
{
}

void AGrid::initGrid(float height) {
	blockCountMax = height;
	height += 3;
	SetActorScale3D(FVector(1, 1, height) * 1.5);
	SetActorLocation(GetActorLocation() + FVector(0, 0, (height / 2) * 100));
}

bool AGrid::canSpawn()
{
	return state == gridState::HasSpace;
}

void AGrid::setState()
{
	if (item == nullptr) {
		if (blockCount == blockCountMax)
			state = gridState::FULL;
		else
			state = gridState::HasSpace;
	}
	else
	{
		state = gridState::Busy;
	}
}

void AGrid::SpawnMarker()
{
	FVector spawn = FVector(GetActorLocation().X, GetActorLocation().Y, (blockCount * 100));
	marker = GetWorld()->SpawnActor<AMarker>(AMarker::StaticClass(), spawn, FRotator::ZeroRotator);
}

void AGrid::setBusy()
{
	state = gridState::Busy;
	SpawnMarker();
}


// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (item != nullptr && !item->GetFalling()) {
		if (marker != nullptr) {
			marker->Destroy();
			marker = nullptr;
		}
		item = nullptr;
		TArray<AActor*> items;
		GetOverlappingActors(items, AItem::StaticClass());

		for (auto i : items) {
			if (i != nullptr && Cast<AItem>(i)->GetFalling()) {
				item = Cast<AItem>(i);
			}
		}
		setState();
	}
}