// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSItemDestroyedCounter.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STACKATTACKCPP_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

private:

	class ASPlayerState				*PlayerState			= nullptr;
	class ASCharacter				*Character				= nullptr;

	class USPlayerUIWidget			*PlayerUIWidget			= nullptr;
	class USMainMenu				*MainMenuWidget			= nullptr;

	FTimerHandle					UpdateUITimer;

	void UpdateUI();

public:

	ASPlayerController();

	UFUNCTION()
	void		UpdateStruct(const FSItemDestroyedCounter &str);

	void		UpdateHealth		(int);
	void		UpdatePoints		(int);
	void		UpdateCoin			(int);
	void		SetPush				(bool);
	void		SetTake				(bool, FText);

	void		SetPlayerUIWG		(class USPlayerUIWidget *w);
	void		TogglePauseMenu();

	void		InitController();

protected:
	virtual void BeginPlay() override;
};
