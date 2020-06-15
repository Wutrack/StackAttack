// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

USTRUCT()
struct FSaveCharacter {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FVector Location;
	UPROPERTY(EditAnywhere)
	int32 Rotation;

	UPROPERTY(EditAnywhere)
	int32 Health;
	UPROPERTY(EditAnywhere)
	int32 HealthMax;
};


UCLASS()
class STACKATTACKCPP_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

	class ASPlayerController	*Controller					= nullptr;

	int32						ZoomMin						= 300, 
								ZoomMax						= 1000;
	FVector						CameraInput					= FVector(0);

	class USFadeObjects			*FadeObjectsComponent		= nullptr;

	FTimerHandle				TraceHandle;
	float						TraceLenght					= 120;
	bool						bCanPushItem				= false, 
								bCanTakeItem				= false;
	FString						TakeText;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjTypes;

	//Variable for item Highlight
	UStaticMeshComponent		*CurrentHighLightMesh		= nullptr;
	FHitResult					Hit;

	//Setup Health
	int32						Health						= 3, 
								HealthMax					= 3;

	void			PushItem();
	void			TakeItem();

	//On player dead Logic
	FTimerHandle	RotateTHandle;
	void			KillPlayer();
	void			RotateCameraAfterDeath();

	void			BackToMainMenu();

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UAudioComponent				*AudioComponent;
	UPROPERTY(EditAnywhere)
	USoundBase					*DeadSound;
	UPROPERTY(EditAnywhere)
	USoundBase					*HitSound;
	UPROPERTY(EditAnywhere)
	USoundBase					*HealSound;

public:	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);


	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent		*Arm;


	UPROPERTY(VisibleAnywhere)
	class UCameraComponent			*Camera;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent (class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void			MoveForward		(float f);

	UFUNCTION()
	void			MoveRight		(float f);

	UFUNCTION()
	void			PitchCamera		(float f);

	UFUNCTION()
	void			YawCamera		(float f);
	
	UFUNCTION()
	void			ZoomIn();

	UFUNCTION()
	void			ZoomOut();

	UFUNCTION()
	void			StartJump();

	UFUNCTION()
	void			StopJump();

	void			CheckForItemsAhead();

	int32			GetHealth() const		{ return Health; };
	int32			GetHealthMax() const	{ return HealthMax; };

	void			SetHealth				(int);
	void			HitPlayer				(int);
	void			HealPlayer				(int);

	const FSaveCharacter	SaveCharacter() const;
	void					LoadCharacter				(const FSaveCharacter &);
};
