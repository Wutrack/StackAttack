// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class STACKATTACKCPP_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

	class ASaveLoadManager *SLManager;
	class UCPlayerUIWidget *playerUI;

protected:

	virtual void BeginPlay() override;

public: 
	AMainGameMode();
};
