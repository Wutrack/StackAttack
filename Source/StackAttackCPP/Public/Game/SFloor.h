// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSItemDestroyedCounter.h"
#include "GameFramework/Actor.h"
#include "SFloor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemsDestroy, const FSItemDestroyedCounter&, Struct);

UCLASS()
class STACKATTACKCPP_API ASFloor : public AActor
{
	GENERATED_BODY()

private:
	UStaticMeshComponent	*PlatformMesh	= nullptr, 
							*PlaneMesh	= nullptr;
	USceneComponent			*SceneRoot	= nullptr;
	UStaticMeshComponent	*Walls[4];

	int32					StackHeight,
							RoomWidth,
							GridSize;

	class ASGrid				***GridArr;

	FSItemDestroyedCounter	ItemStruct;

	//Must to change it
	void DestroyItem		(AActor *);	
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

public:	

	// Sets default values for this actor's properties
	ASFloor();
	void			InitializeFloor			(const int &, const int &, const int &);
	void			SetGridBusy				(const FVector&);
	FVector			GetDestVector();

	void			CheckLinesDestroy		(FVector);

	UPROPERTY()
	FOnItemsDestroy OnItemDestroy;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
