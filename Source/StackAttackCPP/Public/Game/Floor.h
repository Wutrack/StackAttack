// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FItemDestroyed.h"
#include "GameFramework/Actor.h"
#include "Floor.generated.h"



UCLASS()
class STACKATTACKCPP_API AFloor : public AActor
{
	GENERATED_BODY()

	UStaticMeshComponent *PlatformMesh, *PlaneMesh;
	USceneComponent *SceneRoot;
	UStaticMeshComponent *Walls[4];

	int32 StackHeight;
	int32 RoomWidth;
	int32 GridSize;
	class AGrid ***GridArr;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	FItemDestroyed ItemStruct;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	FVector Start, End;

	void DestroyItem(AActor *);
public:	
	// Sets default values for this actor's properties
	AFloor();
	void InitFloor(const int &, const int &, const int &);
	void SetGridBusy(const FVector&);
	FVector GetDestVector();

	void CheckLinesDestroy(FVector);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
