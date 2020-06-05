// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

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
class STACKATTACKCPP_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

	class ACPlayerController *Controller;

	int32 ZoomMin, ZoomMax;
	FVector CameraInput;

	class UFadeObjects *FadeObj;

	FTimerHandle TraceHandle;
	FVector TraceStart, TraceEnd;
	float TraceLenght;
	bool bMove, bTake;
	FString TakeText;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjTypes;
	UStaticMeshComponent *CurrMeshTrace;
	FHitResult Hit;

	int32 Health, HealthMax;

	void PushItem();
	void TakeItem();

	void KillPlayer();

	//temp Function
	void ToMainMenu();

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	


	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent *Arm;


	UPROPERTY(VisibleAnywhere)
	class UCameraComponent *Camera;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveForward(float f);

	UFUNCTION()
	void MoveRight(float f);

	UFUNCTION()
	void PitchCamera(float f);

	UFUNCTION()
	void YawCamera(float f);
	
	UFUNCTION()
	void ZoomIn();

	UFUNCTION()
	void ZoomOut();

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	void Trace();

	int32 GetHealth() const { return Health; };
	int32 GetHealthMax() const { return HealthMax; };

	void SetHealth(int);
	void HitPlayer(int);
	void HealPlayer(int);

	const FSaveCharacter SaveCharacter() const;
	void LoadCharacter(const FSaveCharacter &);
};
