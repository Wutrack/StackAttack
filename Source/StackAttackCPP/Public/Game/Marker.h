// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Marker.generated.h"

UCLASS()
class STACKATTACKCPP_API AMarker : public AActor
{
	GENERATED_BODY()
	
private:
	
	FString gtfPath, fallPass;
	
	UStaticMeshComponent *Mesh;
	class UMaterialInstance *Mat1, *Mat2;

public:	
	// Sets default values for this actor's properties
	AMarker();

	bool bFall;
	void changeMat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
