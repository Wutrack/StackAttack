// Fill out your copyright notice in the Description page of Project Settings.


#include "SFloor.h"
#include "SGrid.h"
#include "SItemBase.h"
#include "SPlayerController.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/EngineTypes.h"

#include "DrawDebugHelpers.h"

void ASFloor::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)
		&& Cast<ASItemBase>(OtherActor)) {
		CheckLinesDestroy(OtherActor->GetActorLocation());
	}
}

void ASFloor::CheckLinesDestroy(FVector vec)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

	FVector Start = FVector(-50, vec.Y, 50);
	FVector End = Start + FVector((RoomWidth) * GridSize, 0, 0);
	TArray<FHitResult> hitArr1;

	GetWorld()->LineTraceMultiByObjectType(hitArr1, Start, End, TraceObjectTypes);

	//DrawDebugLine(GetWorld(), start, end, FColor::Purple, false, 1, 0, 1);

	Start = FVector(vec.X, -50, 50);
	End = Start + FVector(0, (RoomWidth) * GridSize, 0);
	TArray<FHitResult> hitArr2;

	GetWorld()->LineTraceMultiByObjectType(hitArr2, Start, End, TraceObjectTypes);

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
		OnItemDestroy.Broadcast(ItemStruct);
		ItemStruct.Clear();
	}	
}

void ASFloor::DestroyItem(AActor *item) {
	ItemStruct.AddItem(Cast<ASItemBase>(item));
	Cast<ASItemBase>(item)->Destroy();
}

// Sets default values
ASFloor::ASFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh"));

	RootComponent = SceneRoot;
	PlatformMesh->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
	PlaneMesh->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlatformMeshFinder(TEXT("/Engine/BasicShapes/Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshFinder(TEXT("/Engine/BasicShapes/Plane"));

	if (PlatformMeshFinder.Succeeded()) {
		PlatformMesh->SetStaticMesh(PlatformMeshFinder.Object);
	}
	if (PlaneMeshFinder.Succeeded()) {
		PlaneMesh->SetStaticMesh(PlaneMeshFinder.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface> PlatformMaterialFinder(TEXT("/Game/Materials/InstanceMaterial/Platform_Inst.Platform_Inst"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> PlaneMaterialFinder(TEXT("/Game/Materials/InstanceMaterial/Plane_Inst.Plane_Inst"));

	if (PlatformMaterialFinder.Succeeded()) {
		PlatformMesh->SetMaterial(0, Cast<UMaterialInterface>(PlatformMaterialFinder.Object));
	}
	if (PlaneMaterialFinder.Succeeded()) {
		PlaneMesh->SetMaterial(0, Cast<UMaterialInterface>(PlaneMaterialFinder.Object));
	}

	for (size_t i = 0; i < 4; i++)
	{
		FName name = "Wall" + i;
		Walls[i] = CreateDefaultSubobject<UStaticMeshComponent>(name);

		ConstructorHelpers::FObjectFinder<UStaticMesh>WallMechFinder(TEXT("/Engine/BasicShapes/Plane"));
		if (WallMechFinder.Succeeded()) {
			Walls[i]->SetStaticMesh(WallMechFinder.Object);
		}	
		ConstructorHelpers::FObjectFinder<UMaterialInterface> WallMaterialFinder(TEXT("/Game/Materials/InstanceMaterial/Walli.Walli"));
		if (WallMaterialFinder.Succeeded()) {
			Walls[i]->SetMaterial(0, Cast<UMaterialInterface>(WallMaterialFinder.Object));
		}

		Walls[i]->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
		Walls[i]->SetCollisionObjectType(ECC_WorldStatic);
	}
	
	PlatformMesh->OnComponentBeginOverlap.AddDynamic(this, &ASFloor::OnOverlapBegin);
	ItemStruct.Clear();
}

void ASFloor::InitializeFloor(const int &RoomWidthIn, const int &GridSizeIn, const int &StackHeightIn)
{
	StackHeight = StackHeightIn;
	RoomWidth = RoomWidthIn;
	GridSize = GridSizeIn;

	PlatformMesh->SetRelativeScale3D(FVector(RoomWidth, RoomWidth, 1));
	PlatformMesh->SetRelativeLocation(FVector(GridSize / 2 * RoomWidth - GridSize / 2, GridSize / 2 * RoomWidth - GridSize / 2, -GridSize / 2));

	PlaneMesh->SetRelativeScale3D(FVector(RoomWidth * 2, RoomWidth * 2, 1));
	PlaneMesh->SetRelativeLocation(FVector(GridSize / 2 * RoomWidth - GridSize / 2, GridSize / 2 * RoomWidth - GridSize / 2, -GridSize));
		
	Walls[0]->SetRelativeScale3D(FVector(RoomWidth, StackHeight + 3, 1));
	Walls[0]->SetRelativeLocation(FVector(GridSize / 2 * RoomWidth - GridSize / 2, -GridSize / 2,
		(StackHeight + 3) * GridSize / 2));
	Walls[0]->SetRelativeRotation(FRotator(0, 0, 90));

	Walls[1]->SetRelativeScale3D(FVector(RoomWidth, StackHeight + 3, 1));
	Walls[1]->SetRelativeLocation(FVector(GridSize * RoomWidth - GridSize / 2, GridSize / 2 * RoomWidth - GridSize / 2,
		(StackHeight + 3) * GridSize / 2));
	Walls[1]->SetRelativeRotation(FRotator(0, 90, 90));

	Walls[2]->SetRelativeScale3D(FVector(RoomWidth, StackHeight + 3, 1));
	Walls[2]->SetRelativeLocation(FVector(GridSize / 2 * RoomWidth - GridSize / 2, GridSize * RoomWidth - GridSize / 2,
		(StackHeight + 3) * GridSize / 2));
	Walls[2]->SetRelativeRotation(FRotator(0, 180, 90));

	Walls[3]->SetRelativeScale3D(FVector(RoomWidth, StackHeight + 3, 1));
	Walls[3]->SetRelativeLocation(FVector(-GridSize / 2, GridSize / 2 * RoomWidth - GridSize / 2,
		(StackHeight + 3) * GridSize / 2));
	Walls[3]->SetRelativeRotation(FRotator(0, 270, 90));

	GridArr = new ASGrid**[RoomWidth];

	FVector vec;

	for (int i = 0; i < RoomWidth; i++) {
		GridArr[i] = new ASGrid*[RoomWidth];
		for (int j = 0; j < RoomWidth; j++)
		{
			vec = FVector(GridSize * i, GridSize * j, 0);

			GridArr[i][j] = GetWorld()->SpawnActor<ASGrid>
				(ASGrid::StaticClass(), vec, FRotator::ZeroRotator);
			GridArr[i][j]->InitializeGrid(StackHeight);
		}
	}
}


// Called when the game starts or when spawned
void ASFloor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASFloor::GetDestVector() {

	int x, y;

	do {
		x = rand() % RoomWidth;
		y = rand() % RoomWidth;
		if (x == 0 && y == 0) x++;
	} while (!GridArr[x][y]->CanSpawn());

	return FVector((GridSize * x), (GridSize * y), (StackHeight + 1) * GridSize);
}

void ASFloor::SetGridBusy(const FVector &vec)
{
	int32 x = vec.X / 100;
	int32 y = vec.Y / 100;

	GridArr[x][y]->SetBusy();
}

