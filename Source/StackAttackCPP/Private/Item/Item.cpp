// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Floor.h"
#include "MainCharacter.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"
#include <cmath>

void AItem::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		//If other actor is an Item - stop falling and set position
		if (Cast<AItem>(OtherActor) && Cast<AItem>(OtherActor)->GetFalling() == false &&
			OtherActor->GetActorLocation().Z < this->GetActorLocation().Z) {
			BottomItem = OtherActor;

			if (bFalling) {
				StopFalling();
				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y,
					OtherActor->GetActorLocation().Z + 100));
			}
		}
		//If other actor is a Player - Hit end destroy
		else if (GetWorld()->GetFirstPlayerController()->GetCharacter() == Cast<AMainCharacter>(OtherActor)) {
			Cast<AMainCharacter>(OtherActor)->HitPlayer(HitDamage);
			Destroy();
		}
	}
}

void AItem::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (Cast<AItem>(OtherActor) && OtherActor->GetActorLocation().Z < this->GetActorLocation().Z) {

			//If bottom item was destroyed - start falling
			if (BottomItem == nullptr) {
				StartFalling();
			}
			//If other actor didn`t changed - start falling 
			else if (BottomItem != nullptr && BottomItem == OtherActor) {
				StartFalling();
			}
		}
	}
}

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BottomItem = nullptr; 
	Floor = nullptr;

	bFresh = true;
	bAttached = false;
	bFalling = false;
	bLanded = false;
	bMoving = false;
	bTake = false;
	Points = 10;
	HitDamage = 1; 
	FallSpeedStart = 1;
	FallSpeed = FallSpeedStart;
	FallBoost = 1.02;

	MoveSpeed = 500;

	//Create static mesh and box for overlaps events 
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshItem"));
	ItemBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollItem"));

	//Set static mesh as root
	RootComponent = ItemMesh;

	//Set collision box
	ItemBox->AttachToComponent(ItemMesh, FAttachmentTransformRules::KeepRelativeTransform);
	ItemBox->SetRelativeLocation(FVector(0, 0, -3));
	ItemBox->SetRelativeScale3D(FVector(1.55, 1.55, 1.6));
	

	//Ignnore all while in top area to prevent overlap event
	ItemMesh->SetCollisionProfileName("IgnoreAll");
	ItemBox->SetCollisionProfileName("IgnoreAll");
	ItemMesh->SetGenerateOverlapEvents(false);
	ItemBox->SetGenerateOverlapEvents(false);

	ItemBox->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	ItemBox->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);

	//Define audio component
	SoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DestroyAudio"));
	SoundComponent->bAutoActivate = false;
	SoundComponent->AttachToComponent(ItemMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<USoundBase> 
		MoveSoundBase(TEXT("/Game/StarterContent/Audio/Steam01.Steam01"));
	static ConstructorHelpers::FObjectFinder<USoundBase> 
		DestroySoundBase(TEXT("/Game/StarterContent/Audio/Collapse02.Collapse02"));
	static ConstructorHelpers::FObjectFinder<USoundBase> 
		FallSoundBase(TEXT("/Game/Item/Audio/FallSound01.FallSound01"));

	if (MoveSoundBase.Succeeded()) {
		MoveSound = MoveSoundBase.Object;
	}
	if (DestroySoundBase.Succeeded()) {
		FallSound = FallSoundBase.Object;
	}
	if (FallSoundBase.Succeeded()) {
		DestroySound = DestroySoundBase.Object;
	}
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	//Set Item`s static mesh
	ItemMesh->SetStaticMesh(MeshArrBP[0]);

	//Set Item`s Material
	ItemMesh->SetMaterial(0, MaterialArrBP[rand() % MaterialArrBP.Num()]);

}


void AItem::OnAttach()
{
	bFresh = false;
	bAttached = true;
}

void AItem::OnDetach()
{
	bAttached = false;
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	StartFalling();

	//Set normal collision
	ItemBox->SetGenerateOverlapEvents(true);
	ItemMesh->SetGenerateOverlapEvents(true);
	ItemBox->SetCollisionProfileName("Item");
	ItemMesh->SetCollisionProfileName("BlockAll");
}


void AItem::StartFalling()
{
	bFalling = true;
	bLanded = false;
	GetWorld()->GetTimerManager().SetTimer(FallingTHandle, this, &AItem::Falling, 0.01, true);
}

void AItem::Falling()
{
	//Increase falling speed each time
	FallSpeed *= FallBoost;

	AddActorLocalOffset(FVector(0, 0, -FallSpeed));

	//Check if item on ground
	if (GetActorLocation().Z <= 50) {
		StopFalling();
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 50));
	}
}
void AItem::StopFalling()
{
	bFalling = false;
	bLanded = true;

	//Set default falling speed
	FallSpeed = FallSpeedStart;
	//Stop falling timer
	GetWorld()->GetTimerManager().ClearTimer(FallingTHandle);
	//Play fall sound
	SoundComponent->SetSound(FallSound);
	SoundComponent->Play();
}



//Push Item logic
bool AItem::CanPush(const FVector playerVec)
{
	//Check if item dont moving
	if (!bLanded || bMoving || bFalling) return false;

	TraceStart = GetActorLocation();

	if (abs(playerVec.Z - TraceStart.Z) > 100) return false;
	
	if (abs(playerVec.X - TraceStart.X) <= 50)
	{
		if (playerVec.Y - TraceStart.Y < 0)
			TraceEnd = TraceStart + FVector(0, 100, 100);
		else
			TraceEnd = TraceStart + FVector(0, -100, 100);
	}
	else if (abs(playerVec.Y - TraceStart.Y) <= 50)
	{
		if (playerVec.X - TraceStart.X < 0)
			TraceEnd = TraceStart + FVector(100, 0, 100);
		else
			TraceEnd = TraceStart + FVector(-100, 0, 100);
	}
	else return false;

	FHitResult hit;
	Fquat = GetActorQuat();

	TArray<TEnumAsByte<EObjectTypeQuery>> col;
	col.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

	FCollisionQueryParams param;
	param.AddIgnoredActor(this);

	//Send trace in diagonal direction to check for another item or wall 
	GetWorld()->SweepSingleByObjectType(hit, TraceStart, TraceEnd, Fquat, col,
		FCollisionShape::MakeSphere(10.0f), param);

	return !hit.bBlockingHit;
}

void AItem::Push(const FVector vec)
{
	if (CanPush(vec)) {

		SoundComponent->SetSound(MoveSound);
		SoundComponent->Play();

		MoveDestVec = FVector(TraceEnd.X, TraceEnd.Y, GetActorLocation().Z);

		bMoving = true;

		GetWorld()->GetTimerManager().SetTimer(MoveTHandle, this, &AItem::PushItem, 0.01, true);
	}
}

void AItem::PushItem()
{
	const FVector newf = FMath::VInterpConstantTo(GetActorLocation(), MoveDestVec, GetWorld()->DeltaTimeSeconds, MoveSpeed);

	SetActorLocation(newf);
	
	if (newf == MoveDestVec) StopPush();
}

void AItem::StopPush()
{
	SoundComponent->Stop();

	GetWorld()->GetTimerManager().ClearTimer(MoveTHandle);
	bMoving = false;
	if (GetActorLocation().Z == 50 && Floor != nullptr) {
		Floor->CheckLinesDestroy(GetActorLocation());
	}
}

//On Item destroy
void AItem::DestroyItem()
{
	Destroy();
}

//SaveLoad logic
FSaveItemStruct AItem::SaveItem()
{
	FSaveItemStruct save;

	save.NewElement(GetActorLocation(), Type, BoostType, GetClass());
	save.SaveBool(bAttached, bFresh, bLanded);

	if (bFalling) {
		save.SaveFalling(bFalling, FallSpeed);
	}
	else if (bMoving) {
		save.SaveMoving(bMoving, MoveDestVec);
	}

	return save;
}

AItem* AItem::LoadItem(const FSaveItemStruct &load)
{
	bFresh = load.bFresh;
	bAttached = load.bAttached;
	bFalling = load.bFalling;
	bMoving = load.bMoving;
	bLanded = load.bLanded;

	if (!bAttached && !bFresh) 
	{
		GetWorld()->GetTimerManager().SetTimer(TestTHandle, this, &AItem::UpdateOverlapAfterLoading, 0.2, false);
	}

	if (!bLanded) 
	{
		if (bFalling) {
			FallSpeed = load.FallSpeed;
			StartFalling();
		}
		else if (bMoving) {
			MoveDestVec = load.MoveDestVec;
			GetWorld()->GetTimerManager().SetTimer(MoveTHandle, this, &AItem::PushItem, 0.01, true);
		}
	}

	return this;
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::UpdateOverlapAfterLoading() {
	ItemBox->SetCollisionProfileName("Item");
	ItemMesh->SetCollisionProfileName("BlockAll");
	ItemBox->SetGenerateOverlapEvents(true);
	ItemMesh->SetGenerateOverlapEvents(true);
	UpdateOverlaps();
}
