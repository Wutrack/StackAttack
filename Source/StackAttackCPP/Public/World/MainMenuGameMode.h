// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class STACKATTACKCPP_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

	TSubclassOf<class UUserWidget> WGClass;
	class UCMainMenu *MainMenuWidget;

protected:

	virtual void BeginPlay() override;

public: 
	AMainMenuGameMode();
};
