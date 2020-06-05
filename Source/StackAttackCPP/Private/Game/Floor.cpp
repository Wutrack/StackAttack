// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"
#include "Grid.h"
#include "Item.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/EngineTypes.h"
#include "CPlayerController.h"

#include "DrawDebugHelpers.h"

void AFloor::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)
		&& Cast<AItem>(OtherActor)) {
		CheckLinesDestroy(OtherActor->GetActorLocation());
	}
}

void AFloor::CheckLinesDestroy(FVector vec)
{
	Start = FVector(-50, vec.Y, 50);
	End = Start + FVector((RoomWidth) * GridSize, 0, 0);
	TArray<FHitResult> hitArr1;

	GetWorld()->LineTraceMultiByObjectType(hitArr1, Start, End, TraceObjectTypes);

	//DrawDebugLine(GetWorld(), start, end, FColor::Purple, false, 1, 0, 1);

	Start = FVector(vec.X, -50, 50);
	End = Start + FVector(0, (RoomWidth) * GridSize, 0);
	TArray<FHitResult> hitArr2;

	GetWorld()->LineTraceMultiByObjectType(hitArr2, Start, End, TraceObjectTypes);

	//DrawDebugLine(GetWorld(), start, end, FColor::Purple, false, 1, 0, 1);

	if (hitArr1.Num() >= RoomWidth && hitArr2.Num() >= RoomWidth) {

		for (auto i : hitArr1) {
			DestroyItem(i.GetActor());
		}
		for (auto i : hitArr2) {
			if (i.GetActor() != nullptr) {
				DestroyItem(i.GetActor());
			}
		}
	}
	else if (hitArr1.Num() >= RoomWidth) {

		for (auto i : hitArr1) {
			DestroyItem(i.GetActor());
		}
	}
	else if (hitArr2.Num() >= RoomWidth) {

		for (auto i : hitArr2) {
			DestroyItem(i.GetActor());
		}
	}
	if (!ItemStruct.bEmpty) {
		GetWorld()->GetFirstPlayerController<ACPlayerController>()->UpdateStruct(ItemStruct);
		ItemStruct.Clear();
	}
	
}

void AFloor::DestroyItem(AActor *item) {
	ItemStruct.AddItem(Cast<AItem>(item));
	Cast<AItem>(item)->Destroy();
}

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));

	PlatformMesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cube")).Object);
	PlaneMesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Plane")).Object);

	RootComponent = SceneRoot;
	PlatformMesh->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
	PlaneMesh->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);

	PlatformMesh->SetMaterial(0, Cast<UMaterialInterface>
		(ConstructorHelpers::FObjectFinder<UMaterialInstance>
			(TEXT("/Game/Materials/InstanceMaterial/Platform_Inst.Platform_Inst")).Object));
	PlaneMesh->SetMaterial(0, Cast<UMaterialInterface>
		(ConstructorHelpers::FObjectFinder<UMaterialInstance>
			(TEXT("/Game/Materials/InstanceMaterial/Plane_Inst.Plane_Inst")).Object));

	for (size_t i = 0; i < 4; i++)
	{
		FName name = "Wall" + i;
		Walls[i] = CreateDefaultSubobject<UStaticMeshComponent>(name);
		Walls[i]->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Plane")).Object);
		Walls[i]->SetMaterial(0, Cast<UMaterialInterface>
			(ConstructorHelpers::FObjectFinder<UMaterialInstance>
				(TEXT("/Game/Materials/InstanceMaterial/Walli.Walli")).Object));
		Walls[i]->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
		Walls[i]->SetCollisionObjectType(ECC_WorldStatic);
	}
	
	PlatformMesh->OnComponentBeginOverlap.AddDynamic(this, &AFloor::OnOverlapBegin);

	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

	ItemStruct.Clear();
}

void AFloor::InitFloor(const int &RoomWidthIn, const int &GridSizeIn, const int &StackHeightIn)
{
	StackHeight = StackHeightIn;
	RoomWidth = RoomWidthIn;
	GridSize = GridSizeIn;

	PlatformMesh->SetRelativeScale3D(FVector(RoomWidth, RoomWidth, 1));
	PlatformMesh->SetRelativeLocation(FVector(GridSize / 2 * RoomWidth - GridSize / 2, GridSize / 2 * RoomWidth - GridSize / 2, -GridSize / 2));

	PlaneMesh->SetRelativeScale3D(FVector(RoomWidth * 2, RoomWidth * 2, 1));
	PlaneMesh->SetRelativeLocation(FVector(GridSize / 2 * RoomWidth - GridSize / 2, GridSize / 2 * RoomWidth - GridSize / 2, -GridSize));
		
	Walls[0]->SetRelativeScale3D(FVector(RoomWidth, RoomWidth * 2, 1));
	Walls[0]->SetRelativeLocation(FVector(GridSize / 2 * RoomWidth - GridSize / 2, -GridSize / 2,
		(StackHeight + 3) * GridSize / 2));
	Walls[0]->SetRelativeRotation(FRotator(0, 0, 90));

	Walls[1]->SetRelativeScale3D(FVector(RoomWidth, RoomWidth * 2, 1));
	Walls[1]->SetRelativeLocation(FVector(GridSize * RoomWidth - GridSize / 2, GridSize / 2 * RoomWidth - GridSize / 2,
		(StackHeight + 3) * GridSize / 2));
	Walls[1]->SetRelativeRotation(FRotator(0, 90, 90));

	Walls[2]->SetRelativeScale3D(FVector(RoomWidth, RoomWidth * 2, 1));
	Walls[2]->SetRelativeLocation(FVector(GridSize / 2 * RoomWidth - GridSize / 2, GridSize * RoomWidth - GridSize / 2,
		(StackHeight + 3) * GridSize / 2));
	Walls[2]->SetRelativeRotation(FRotator(0, 180, 90));

	Walls[3]->SetRelativeScale3D(FVector(RoomWidth, RoomWidth * 2, 1));
	Walls[3]->SetRelativeLocation(FVector(-GridSize / 2, GridSize / 2 * RoomWidth - GridSize / 2,
		(StackHeight + 3) * GridSize / 2));
	Walls[3]->SetRelativeRotation(FRotator(0, 270, 90));

	GridArr = new AGrid**[RoomWidth];

	FVector vec;

	for (int i = 0; i < RoomWidth; i++) {
		GridArr[i] = new AGrid*[RoomWidth];
		for (int j = 0; j < RoomWidth; j++)
		{
			vec = FVector(GridSize * i, GridSize * j, 0);

			GridArr[i][j] = GetWorld()->SpawnActor<AGrid>
				(AGrid::StaticClass(), vec, FRotator::ZeroRotator);
			GridArr[i][j]->initGrid(StackHeight);
		}
	}
}


// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AFloor::GetDestVector() {

	int x, y;

	do {
		x = rand() % RoomWidth;
		y = rand() % RoomWidth;
		if (x == 0 && y == 0) x++;
	} while (!GridArr[x][y]->canSpawn());
	GridArr[x][y]->setBusy();

	return FVector((GridSize * x), (GridSize * y), (StackHeight + 1) * GridSize);
}

void AFloor::SetGridBusy(const FVector &vec)
{
	int32 x = vec.X / 100;
	int32 y = vec.Y / 100;

	GridArr[x][y]->setBusy();
}

