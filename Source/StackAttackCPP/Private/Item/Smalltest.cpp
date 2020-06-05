// Fill out your copyright notice in the Description page of Project Settings.


#include "Smalltest.h"

// Sets default values
ASmalltest::ASmalltest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASmalltest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASmalltest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

