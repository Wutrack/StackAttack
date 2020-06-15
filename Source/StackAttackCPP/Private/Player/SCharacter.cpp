// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "SPlayerController.h"
#include "SFadeObjects.h"
#include "SItemBase.h"
#include "SCoin.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "math.h"



void ASCharacter::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (ASCoin* Coin = Cast<ASCoin>(OtherActor)) {
		Controller->UpdateCoin(Coin->CoinValue);
		Coin->CoinCollected();
	}
}
// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentHighLightMesh = nullptr;
	CameraInput = FVector();


	Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Arm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(Arm, USpringArmComponent::SocketName);

	FadeObjectsComponent = CreateDefaultSubobject<USFadeObjects>(TEXT("FadeObjectsComponent"));

	ObjTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	//Add OnBeginOverlap event
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASCharacter::OnOverlapBegin);

	//Setup audio component
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		DeadSoundBase(TEXT("/Game/Player/Audio/DeadSound.DeadSound"));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		HitSoundBase(TEXT("/Game/Player/Audio/Hit01.Hit01"));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		HealSoundBase(TEXT("/Game/Player/Audio/Heal01.Heal01"));

	if (DeadSoundBase.Succeeded()) {
		DeadSound = DeadSoundBase.Object;
	}
	if (DeadSoundBase.Succeeded()) {
		HitSound = HitSoundBase.Object;
	}
	if (DeadSoundBase.Succeeded()) {
		HealSound = HealSoundBase.Object;
	}
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Controller = GetController<ASPlayerController>();

	GetWorld()->GetTimerManager().SetTimer(TraceHandle, this, &ASCharacter::CheckForItemsAhead, 0.05f, true);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddControllerYawInput(CameraInput.X);
	{
		FRotator rot = Arm->GetComponentRotation();
		rot.Pitch = FMath::Clamp(rot.Pitch + CameraInput.Y, -80.0f, 70.0f);
		Arm->SetWorldRotation(rot);
	}
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::YawCamera);
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::PitchCamera);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASCharacter::StopJump);

	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &ASCharacter::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &ASCharacter::ZoomOut);

	PlayerInputComponent->BindAction("Push", IE_Pressed, this, &ASCharacter::PushItem);
	PlayerInputComponent->BindAction("TakeItem", IE_Pressed, this, &ASCharacter::TakeItem);
	PlayerInputComponent->BindAction("ToMainMenu", IE_Pressed, this, &ASCharacter::BackToMainMenu).bExecuteWhenPaused = true;
}

void ASCharacter::MoveForward(float f)
{
	if (Controller && f != 0) {
		FVector direc = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(direc, f);
	}
}

void ASCharacter::MoveRight(float f)
{
	if (Controller && f != 0) {
		FVector direc = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(direc, f);
	}
}

void ASCharacter::PitchCamera(float f)
{
	CameraInput.Y = f * 0.8;
}

void ASCharacter::YawCamera(float f)
{
	CameraInput.X = f * 0.8;
}

void ASCharacter::ZoomIn()
{
	if (Arm->TargetArmLength >= ZoomMin) {
		Arm->TargetArmLength -= 50;
	}
}

void ASCharacter::ZoomOut()
{
	if (Arm->TargetArmLength <= ZoomMax) {
		Arm->TargetArmLength += 50;
	}
}

void ASCharacter::StartJump()
{
	bPressedJump = true;
}

void ASCharacter::StopJump()
{
	bPressedJump = false;
}

void ASCharacter::CheckForItemsAhead()
{
	FVector TraceStart = GetActorLocation() + FVector(0, 0, 70);

	FVector TraceEnd = ((Camera->GetForwardVector() * TraceLenght) + TraceStart);
	GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjTypes);

	if (Hit.bBlockingHit && Hit.Component != nullptr && Cast<ASItemBase>(Hit.Actor)) {

		if (IsValid(CurrentHighLightMesh) && CurrentHighLightMesh != Cast<UStaticMeshComponent>(Hit.Component)) {
			CurrentHighLightMesh->SetRenderCustomDepth(false);
		}
		ASItemBase *item = Cast<ASItemBase>(Hit.Actor);
		CurrentHighLightMesh = Cast<UStaticMeshComponent>(Hit.Component);
		CurrentHighLightMesh->SetRenderCustomDepth(true);
		bCanPushItem = item->CanPush(GetActorLocation());
		bCanTakeItem = item->CanTake();
		if (bCanTakeItem && TakeText == "") {
			switch (item->GetBoostType())
			{
			case EBoostType::Heal:
				TakeText = "heal";
			default:
				break;
			}
		}
	}
	else
	{
		if (IsValid(CurrentHighLightMesh) && CurrentHighLightMesh != Cast<UStaticMeshComponent>(Hit.Component))
		{
			CurrentHighLightMesh->SetRenderCustomDepth(false);
		}
		bCanPushItem = false;
		bCanTakeItem = false;
		TakeText = "";
	}
	if (Controller) {
		Controller->SetPush(bCanPushItem);
		Controller->SetTake(bCanTakeItem, FText::FromString(TakeText));
	}
}

void ASCharacter::SetHealth(int inc)
{
	Health += inc;
	if (Health < 1) {
		Health = 0;
		KillPlayer();
	}
	else if (Health > HealthMax) Health = HealthMax;

	if (Controller) {
		Controller->UpdateHealth(Health);
	}
}

void ASCharacter::HitPlayer(int damage)
{
	AudioComponent->SetSound(HitSound);
	AudioComponent->Play();
	SetHealth(-damage);
}

void ASCharacter::HealPlayer(int heal)
{
	AudioComponent->SetSound(HealSound);
	AudioComponent->Play();
	SetHealth(heal);
}


void ASCharacter::KillPlayer()
{
	AudioComponent->SetSound(DeadSound);
	AudioComponent->Play();

	UCapsuleComponent* capsule = GetCapsuleComponent();
	capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	capsule->SetCollisionResponseToAllChannels(ECR_Ignore);

	SetActorEnableCollision(true);

	USkeletalMeshComponent *mesh = GetMesh();
	mesh->AddRelativeLocation(FVector(0, 0, 200));
	mesh->SetCollisionProfileName(TEXT("Ragdoll"));
	mesh->SetAllBodiesSimulatePhysics(true);
	mesh->SetSimulatePhysics(true);
	mesh->WakeAllRigidBodies();
	mesh->bBlendPhysics = true;

	UCharacterMovementComponent *moveComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	moveComp->StopMovementImmediately();
	moveComp->DisableMovement();
	moveComp->SetComponentTickEnabled(false);

	UGameplayStatics::DeleteGameInSlot("New", 0);
	Controller->SetInputMode(FInputModeUIOnly());

	GetWorld()->GetTimerManager().SetTimer(RotateTHandle, this, &ASCharacter::RotateCameraAfterDeath, 0.01, true, 0.1);
}

void ASCharacter::RotateCameraAfterDeath()
{
	AddControllerYawInput(0.2);
}

void ASCharacter::BackToMainMenu()
{
	if (Controller) {
		Controller->TogglePauseMenu();
	}
}

//Push item if you can
void ASCharacter::PushItem()
{
	if (Hit.bBlockingHit && CanJump() && Cast<ASItemBase>(Hit.GetActor()))
		Cast<ASItemBase>(Hit.GetActor())->Push(GetActorLocation());
}

//Take Boost item
void ASCharacter::TakeItem()
{
	if (Hit.bBlockingHit && CanJump() && Cast<ASItemBase>(Hit.GetActor())) {
		ASItemBase *Item = Cast<ASItemBase>(Hit.Actor);
		if (Item->GetBoostType() == EBoostType::Heal) {
			HealPlayer(Item->GetBoostPower());
			Item->DestroyItem();
		}
		else if (Item->GetBoostType() == EBoostType::IncMaxHealth) {

		}
	}
}

//Save Load logic
const FSaveCharacter ASCharacter::SaveCharacter() const
{
	FSaveCharacter save;
	save.Location = GetActorLocation();
	save.Rotation = GetActorRotation().Yaw;

	save.Health = Health;
	save.HealthMax = HealthMax;

	return save;
}

void ASCharacter::LoadCharacter(const FSaveCharacter &load)
{
	SetActorLocation(load.Location);
	AddControllerYawInput(load.Rotation / 2.5);

	Health = load.Health;
	HealthMax = load.HealthMax;

	Controller = GetController<ASPlayerController>();
}