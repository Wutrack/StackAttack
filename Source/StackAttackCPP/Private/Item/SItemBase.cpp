// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemBase.h"
#include "SFloor.h"
#include "SCoin.h"
#include "SCharacter.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"
#include <cmath>

void ASItemBase::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		//If other actor is an Item - stop falling and set position
		if (Cast<ASItemBase>(OtherActor) && Cast<ASItemBase>(OtherActor)->GetFalling() == false &&
			OtherActor->GetActorLocation().Z < this->GetActorLocation().Z) {
			BottomItem = OtherActor;

			if (bFalling) {
				StopFalling();
				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y,
					OtherActor->GetActorLocation().Z + 100));
			}
		}
		//If other actor is a Player - Hit end destroy
		else if (Cast<ASCharacter>(OtherActor) && OtherActor->GetActorLocation().Z < GetActorLocation().Z) {
			Cast<ASCharacter>(OtherActor)->HitPlayer(HitDamage);
			Destroy();
		}
		else if (Cast<ASCoin>(OtherActor) && OtherActor->GetActorLocation().Z < GetActorLocation().Z) {
			OtherActor->Destroy();
		}
	}
}

void ASItemBase::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (Cast<ASItemBase>(OtherActor) && OtherActor->GetActorLocation().Z < this->GetActorLocation().Z) {

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
ASItemBase::ASItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Floor = nullptr;

	Points = 10;
	HitDamage = 1; 

	//Create static mesh and box for overlaps events 
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	CollisionItemBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionItemBox"));

	//Set static mesh as root
	RootComponent = ItemMesh;

	//Set collision box
	CollisionItemBox->AttachToComponent(ItemMesh, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionItemBox->SetRelativeLocation(FVector(0, 0, -3));
	CollisionItemBox->SetRelativeScale3D(FVector(1.55, 1.55, 1.6));

	//Ignnore all while in top area to prevent overlap event
	ItemMesh->SetCollisionProfileName("IgnoreAll");
	CollisionItemBox->SetCollisionProfileName("IgnoreAll");
	ItemMesh->SetGenerateOverlapEvents(false);
	CollisionItemBox->SetGenerateOverlapEvents(false);

	//Add OnOverlap events
	CollisionItemBox->OnComponentBeginOverlap.AddDynamic(this, &ASItemBase::OnOverlapBegin);
	CollisionItemBox->OnComponentEndOverlap.AddDynamic(this, &ASItemBase::OnOverlapEnd);

	//Define audio component
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->AttachToComponent(ItemMesh, FAttachmentTransformRules::KeepRelativeTransform);

	//Search and load sounds
	static ConstructorHelpers::FObjectFinder<USoundBase>
		PushSoundFinder(TEXT("/Game/StarterContent/Audio/Steam01.Steam01"));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		LandedSoundFinder(TEXT("/Game/Item/Audio/FallSound01.FallSound01"));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		DestroySoundFinder(TEXT("/Game/StarterContent/Audio/Collapse02.Collapse02"));

	if (PushSoundFinder.Succeeded()) {
		PushSound = PushSoundFinder.Object;
	}
	if (DestroySoundFinder.Succeeded()) {
		LandedSound = LandedSoundFinder.Object;
	}
	if (LandedSoundFinder.Succeeded()) {
		DestroyedSound = DestroySoundFinder.Object;
	}
}

// Called when the game starts or when spawned
void ASItemBase::BeginPlay()
{
	Super::BeginPlay();

	//Set Item`s static mesh
	ItemMesh->SetStaticMesh(MeshCollection[0]);

	//Set Item`s Material
	ItemMesh->SetMaterial(0, MaterialCollection[rand() % MaterialCollection.Num()]);

}


void ASItemBase::OnAttachToCrane()
{
	bJustSpawned = false;
	bAttachedToCrane = true;
}

void ASItemBase::OnDetachFromCrane()
{
	bAttachedToCrane = false;
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	StartFalling();

	//Set normal collision
	CollisionItemBox->SetGenerateOverlapEvents(true);
	ItemMesh->SetGenerateOverlapEvents(true);
	CollisionItemBox->SetCollisionProfileName("Item");
	ItemMesh->SetCollisionProfileName("BlockAll");
}


void ASItemBase::StartFalling()
{
	bFalling = true;
	bDormancy = false;
	GetWorld()->GetTimerManager().SetTimer(FallingTHandle, this, &ASItemBase::Falling, 0.01, true);
}

void ASItemBase::Falling()
{
	//Increase falling speed each time
	FallSpeedCurrent *= FallSpeedBoost;

	AddActorLocalOffset(FVector(0, 0, -FallSpeedCurrent));

	//Check if item on ground
	if (GetActorLocation().Z <= 50) {
		StopFalling();
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 50));
	}
}
void ASItemBase::StopFalling()
{
	bFalling = false;
	bDormancy = true;

	//Set default falling speed
	FallSpeedCurrent = FallSpeedStart;
	//Stop falling timer
	GetWorld()->GetTimerManager().ClearTimer(FallingTHandle);
	//Play fall sound
	AudioComponent->SetSound(LandedSound);
	AudioComponent->Play();
}



//Push Item logic
bool ASItemBase::CanPush(const FVector playerVec)
{
	//Check if item dont moving
	if (!bDormancy || bPushing || bFalling) return false;

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

void ASItemBase::Push(const FVector vec)
{
	if (CanPush(vec)) {

		AudioComponent->SetSound(PushSound);
		AudioComponent->Play();

		MoveDestVec = FVector(TraceEnd.X, TraceEnd.Y, GetActorLocation().Z);

		bPushing = true;

		GetWorld()->GetTimerManager().SetTimer(MoveTHandle, this, &ASItemBase::PushItem, 0.01, true);
	}
}

void ASItemBase::PushItem()
{
	const FVector newf = FMath::VInterpConstantTo(GetActorLocation(), MoveDestVec, GetWorld()->DeltaTimeSeconds, PushSpeed);

	SetActorLocation(newf);
	
	if (newf == MoveDestVec) StopPush();
}

void ASItemBase::StopPush()
{
	AudioComponent->Stop();

	GetWorld()->GetTimerManager().ClearTimer(MoveTHandle);
	bPushing = false;
	if (GetActorLocation().Z == 50 && Floor != nullptr) {
		Floor->CheckLinesDestroy(GetActorLocation());
	}
}

//On Item destroy
void ASItemBase::DestroyItem()
{
	Destroy();
}

//SaveLoad logic
FSaveItemStruct ASItemBase::SaveItem()
{
	FSaveItemStruct save;

	save.NewElement(GetActorLocation(), ItemType, BoostType, GetClass());
	save.SaveBool(bAttachedToCrane, bJustSpawned, bDormancy);

	if (bFalling) {
		save.SaveFalling(bFalling, FallSpeedCurrent);
	}
	else if (bPushing) {
		save.SaveMoving(bPushing, MoveDestVec);
	}

	return save;
}

ASItemBase* ASItemBase::LoadItem(const FSaveItemStruct &load)
{
	bJustSpawned = load.bJustSpawned;
	bAttachedToCrane = load.bAttachedToCrane;
	bFalling = load.bFalling;
	bPushing = load.bPushing;
	bDormancy = load.bDormancy;

	if (!bAttachedToCrane && !bJustSpawned)
	{
		GetWorld()->GetTimerManager().SetTimer(TestTHandle, this, &ASItemBase::UpdateOverlapAfterLoading, 0.2, false);
	}

	if (!bDormancy)
	{
		if (bFalling) {
			FallSpeedCurrent = load.FallSpeedCurrent;
			StartFalling();
		}
		else if (bPushing) {
			MoveDestVec = load.PushDestination;
			GetWorld()->GetTimerManager().SetTimer(MoveTHandle, this, &ASItemBase::PushItem, 0.01, true);
		}
	}

	return this;
}

// Called every frame
void ASItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASItemBase::UpdateOverlapAfterLoading() {
	CollisionItemBox->SetCollisionProfileName("Item");
	ItemMesh->SetCollisionProfileName("BlockAll");
	CollisionItemBox->SetGenerateOverlapEvents(true);
	ItemMesh->SetGenerateOverlapEvents(true);
	UpdateOverlaps();
}
