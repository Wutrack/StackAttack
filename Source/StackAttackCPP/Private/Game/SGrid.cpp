// Fill out your copyright notice in the Description page of Project Settings.


#include "SGrid.h"
#include "SItemBase.h"
#include "SMarker.h"
#include "Engine/Engine.h"
#include "Components/BoxComponent.h"

void ASGrid::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) &&
		Cast<ASItemBase>(OtherActor))
	{
		if (Cast<ASItemBase>(OtherActor)->GetFalling()) {

			PrimaryActorTick.SetTickFunctionEnable(true);

			IncomingItem = Cast<ASItemBase>(OtherActor);
			if (Marker == nullptr) {
				SpawnMarker();
			}
			if (Marker != nullptr) {
				Marker->ChangeMat();
			}
		}
		else {
			if (Marker != nullptr) {
				Marker->AddActorLocalOffset(FVector(0, 0, 100));
			}
		}
		SetState();
		ItemCountCurrent++;
	}
}

void ASGrid::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) 
		&& Cast<ASItemBase>(OtherActor))
	{
		if (IsValid(Marker))
			Marker->AddActorLocalOffset(FVector(0, 0, -100));
		ItemCountCurrent--;
		SetState();
	}
}

ASGrid::ASGrid()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

	RootComponent = CollisionBox;

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASGrid::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ASGrid::OnOverlapEnd);

	CollisionBox->SetCollisionProfileName("Grid");
}

ASGrid::~ASGrid()
{
}

void ASGrid::InitializeGrid(float height) {
	ItemCountMax = height;
	height += 3;
	SetActorScale3D(FVector(1, 1, height) * 1.5);
	SetActorLocation(GetActorLocation() + FVector(0, 0, (height / 2) * 100));
}

bool ASGrid::CanSpawn()
{
	return GridState== gridState::HasSpace;
}

void ASGrid::SetState()
{
	if (!IsValid(IncomingItem)) {
		if (ItemCountCurrent == ItemCountMax)
			GridState = gridState::FULL;
		else
			GridState = gridState::HasSpace;
	}
	else
	{
		GridState= gridState::Busy;
	}
}

void ASGrid::SpawnMarker()
{
	FVector spawn = FVector(GetActorLocation().X, GetActorLocation().Y, (ItemCountCurrent * 100));
	Marker = GetWorld()->SpawnActor<ASMarker>(ASMarker::StaticClass(), spawn, FRotator::ZeroRotator);
}

void ASGrid::SetBusy()
{
	GridState= gridState::Busy;
	SpawnMarker();
}


// Called when the game starts or when spawned
void ASGrid::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.SetTickFunctionEnable(false);

}

// Called every frame
void ASGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IncomingItem != nullptr && IncomingItem->GetDormancy() || !IsValid(IncomingItem)) {
		IncomingItem = nullptr;
		if (Marker != nullptr) {
			Marker->Destroy();
			Marker = nullptr;
		}
		SetState();
		PrimaryActorTick.SetTickFunctionEnable(false);
	}
}