// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STACKATTACKCPP_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

	class AMainPlayerState *PlayerState;
	class AMainCharacter *Character;

	class UCPlayerUIWidget *PlayerUIWidget;
	class UCMainMenu *MainMenuWidget;

public:

	ACPlayerController();
	void UpdateStruct(const struct FItemDestroyed &);
	void UpdateHealth(int);
	void UpdatePoints(int);
	void SetPush(bool);
	void SetTake(bool, FText);

	void SetPlayerUIWG(class UCPlayerUIWidget *w);
	void TogglePauseMenu();

protected:
	virtual void BeginPlay() override;
};
