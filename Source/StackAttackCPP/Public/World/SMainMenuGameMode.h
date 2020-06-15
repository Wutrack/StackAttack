// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SMainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class STACKATTACKCPP_API ASMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

	TSubclassOf<class UUserWidget>	WGClass;
	class USMainMenu				*MainMenuWidget;

protected:

	virtual void BeginPlay() override;

public: 
	ASMainMenuGameMode();
};
