// Fill out your copyright notice in the Description page of Project Settings.


#include "SMarker.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
ASMarker::ASMarker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MarkerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Marker"));
	MarkerMesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cube")).Object);
	MarkerMesh->SetRelativeScale3D(FVector(0.7, 0.7, 0.3));
	RootComponent = MarkerMesh;
	MarkerMesh->SetCollisionProfileName("IgnoreAll");

	static ConstructorHelpers::FObjectFinder<UMaterialInstance>
		NormalStateMaterialFinder(TEXT("/Game/Materials/InstanceMaterial/Marker_GTF.Marker_GTF"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance>
		ActiveStateMaterialFinder(TEXT("/Game/Materials/InstanceMaterial/Marker_FALL.Marker_FALL"));

	if (NormalStateMaterialFinder.Succeeded()) {
		MarkerMesh->SetMaterial(0, Cast<UMaterialInterface>(NormalStateMaterialFinder.Object));
	}
	if (ActiveStateMaterialFinder.Succeeded()) {
		ActiveMaterial = ActiveStateMaterialFinder.Object;
	}

}

void ASMarker::ChangeMat()
{
	MarkerMesh->SetMaterial(0, Cast<UMaterialInterface>(ActiveMaterial));
	bActive = true;
}

// Called when the game starts or when spawned
void ASMarker::BeginPlay()
{
	Super::BeginPlay();
	//Mesh->SetCollisionProfileName("IgnoreOnlyPawn");
	
}

// Called every frame
void ASMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

