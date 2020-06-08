// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Item.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "FadeObjects.h"
#include "Engine/World.h"
#include "math.h"
#include "CPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "DrawDebugHelpers.h"
// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrMeshTrace = nullptr;
	CameraInput = FVector();

	HealthMax = 3;
	Health = HealthMax;

	ZoomMin = 300;
	ZoomMax = 1000;

	Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Arm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(Arm, USpringArmComponent::SocketName);

	FadeObj = CreateDefaultSubobject<UFadeObjects>(TEXT("FadeObj"));

	TraceLenght = 130;
	ObjTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));


	//Define dead audio component
	SoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DeadAudio"));
	SoundComponent->bAutoActivate = false;
	SoundComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
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
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Controller = GetController<ACPlayerController>();

	GetWorld()->GetTimerManager().SetTimer(TraceHandle, this, &AMainCharacter::Trace, 0.05f, true, 0.1);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
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
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::YawCamera);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::PitchCamera);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::StopJump);

	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &AMainCharacter::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &AMainCharacter::ZoomOut);

	PlayerInputComponent->BindAction("Push", IE_Pressed, this, &AMainCharacter::PushItem);
	PlayerInputComponent->BindAction("TakeItem", IE_Pressed, this, &AMainCharacter::TakeItem);
	PlayerInputComponent->BindAction("ToMainMenu", IE_Pressed, this, &AMainCharacter::ToMainMenu).bExecuteWhenPaused = true;
}

void AMainCharacter::MoveForward(float f)
{
	if (Controller && f != 0) {
		FVector direc = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(direc, f);
	}
}

void AMainCharacter::MoveRight(float f)
{
	if (Controller && f != 0) {
		FVector direc = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(direc, f);
	}
}

void AMainCharacter::PitchCamera(float f)
{
	CameraInput.Y = f * 0.8;
}

void AMainCharacter::YawCamera(float f)
{
	CameraInput.X = f * 0.8;
}

void AMainCharacter::ZoomIn()
{
	if (Arm->TargetArmLength >= ZoomMin)
		Arm->TargetArmLength -= 50;
}

void AMainCharacter::ZoomOut()
{
	if (Arm->TargetArmLength <= ZoomMax)
	Arm->TargetArmLength += 50;
}

void AMainCharacter::StartJump()
{
	bPressedJump = true;
}

void AMainCharacter::StopJump()
{
	bPressedJump = false;
}

void AMainCharacter::Trace()
{
	TraceStart = GetActorLocation() + FVector(0, 0, 70);

	TraceEnd = ((Camera->GetForwardVector() * TraceLenght) + TraceStart);
	GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjTypes);

	if (Hit.bBlockingHit && Hit.Component != nullptr && Cast<AItem>(Hit.Actor)) {

		if (IsValid(CurrMeshTrace) && CurrMeshTrace != Cast<UStaticMeshComponent>(Hit.Component)) {
			CurrMeshTrace->SetRenderCustomDepth(false);
		}
		AItem *item = Cast<AItem>(Hit.Actor);
		CurrMeshTrace = Cast<UStaticMeshComponent>(Hit.Component);
		CurrMeshTrace->SetRenderCustomDepth(true);
		bMove = item->CanPush(GetActorLocation());
		bTake = item->CanTake();
		if (bTake && TakeText == "") {
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
		if (IsValid(CurrMeshTrace))
		{
			CurrMeshTrace->SetRenderCustomDepth(false);
		}
		bMove = false;
		bTake = false;
		TakeText = "";
	}
	if(Controller){
		Controller->SetPush(bMove);
		Controller->SetTake(bTake, FText::FromString(TakeText));
	}
}

void AMainCharacter::SetHealth(int inc)
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

void AMainCharacter::HitPlayer(int damage)
{
	SoundComponent->SetSound(HitSound);
	SoundComponent->Play();
	SetHealth(-damage);
}

void AMainCharacter::HealPlayer(int heal)
{
	SoundComponent->SetSound(HealSound);
	SoundComponent->Play();
	SetHealth(heal);
}


void AMainCharacter::KillPlayer()
{
	SoundComponent->SetSound(DeadSound);
	SoundComponent->Play();

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

	GetWorld()->GetTimerManager().SetTimer(RotateTHandle, this, &AMainCharacter::RotateCameraAfterDeath, 0.01, true, 0.1);
}

void AMainCharacter::RotateCameraAfterDeath()
{
	AddControllerYawInput(0.2);
}

void AMainCharacter::ToMainMenu()
{
	if (Controller) {
		Controller->TogglePauseMenu();
	}
}

void AMainCharacter::PushItem()
{
	if (Hit.bBlockingHit && CanJump() && Cast<AItem>(Hit.GetActor()))
		Cast<AItem>(Hit.GetActor())->Push(GetActorLocation());
}

void AMainCharacter::TakeItem()
{
	if (Hit.bBlockingHit && CanJump() && Hit.GetActor() != nullptr) {
		AItem *Item = Cast<AItem>(Hit.Actor);
		if (Item->GetBoostType() == EBoostType::Heal) {
			HealPlayer(Item->GetBoostPower());
			Item->DestroyItem();
		}
		else if (Item->GetBoostType() == EBoostType::IncMaxHealth) {

		}
	}
}

//Save Load logic
const FSaveCharacter AMainCharacter::SaveCharacter() const
{
	FSaveCharacter save;
	save.Location = GetActorLocation();
	save.Rotation = GetActorRotation().Yaw;

	save.Health = Health;
	save.HealthMax = HealthMax;

	return save;
}

void AMainCharacter::LoadCharacter(const FSaveCharacter &load)
{
	SetActorLocation(load.Location);
	AddControllerYawInput(load.Rotation / 2.5);

	Health = load.Health;
	HealthMax = load.HealthMax;

	Controller = GetController<ACPlayerController>();
	if (Controller) {
		Controller->UpdateHealth(Health);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Purple, "No Controller");
	}
}