// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveLoadManager.generated.h"

UCLASS()
class STACKATTACKCPP_API ASaveLoadManager : public AActor
{
	GENERATED_BODY()


	TArray<class AItem*> FreshItemArr;
	TArray<class AItem*> AttachedItemArr;

public:	
	// Sets default values for this actor's properties
	ASaveLoadManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SaveGame();
	void LoadGame(FString);


};
