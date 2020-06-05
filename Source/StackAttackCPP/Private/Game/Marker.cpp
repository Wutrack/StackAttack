// Fill out your copyright notice in the Description page of Project Settings.


#include "Marker.h"
#include <Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h>
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
AMarker::AMarker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bFall = false;

	gtfPath = "/Game/Materials/InstanceMaterial/Marker_GTF.Marker_GTF";
	fallPass = "/Game/Materials/InstanceMaterial/Marker_FALL.Marker_FALL";
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Marker"));
	RootComponent = Mesh;
	Mesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cube")).Object);
	Mesh->SetRelativeScale3D(FVector(0.7, 0.7, 0.3));

	Mat1 = ConstructorHelpers::FObjectFinder<UMaterialInstance>(*gtfPath).Object;
	Mat2 = ConstructorHelpers::FObjectFinder<UMaterialInstance>(*fallPass).Object;
	Mesh->SetMaterial(0, Cast<UMaterialInterface>(Mat1));

	Mesh->SetCollisionProfileName("IgnoreAll");
}

void AMarker::changeMat()
{
	Mesh->SetMaterial(0, Cast<UMaterialInterface>(Mat2));
	bFall = true;
}

// Called when the game starts or when spawned
void AMarker::BeginPlay()
{
	Super::BeginPlay();
	//Mesh->SetCollisionProfileName("IgnoreOnlyPawn");
	
}

// Called every frame
void AMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

