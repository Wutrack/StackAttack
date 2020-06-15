// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SMainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class STACKATTACKCPP_API ASMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

	class ASSaveLoadManager *SLManager = nullptr;
	class USPlayerUIWidget *playerUI = nullptr;

protected:

	virtual void BeginPlay() override;

public: 

	ASMainGameMode();
};
