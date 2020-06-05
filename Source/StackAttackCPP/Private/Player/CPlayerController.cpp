// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerController.h"
#include "MainPlayerState.h"
#include "MainCharacter.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "CPlayerUIWidget.h"
#include "CMainMenu.h"

ACPlayerController::ACPlayerController()
{
}

void ACPlayerController::BeginPlay() {

	Super::BeginPlay();

	PlayerState = GetPlayerState<AMainPlayerState>();
	Character = Cast<AMainCharacter>(GetCharacter());

	FInputModeGameOnly mode;
	SetInputMode(mode);
}


void ACPlayerController::UpdateStruct(const FItemDestroyed &str)
{
	const FItemDestroyed& temp = PlayerState->UpdateStruct(str);
	UpdatePoints(temp.points);
}


void ACPlayerController::UpdatePoints(int points)
{
	if (PlayerUIWidget) {
		PlayerUIWidget->updatePoints(points);
	}
}

void ACPlayerController::UpdateHealth(int health)
{
	if (PlayerUIWidget) {
		PlayerUIWidget->updateHealth(health);
		if (health == 0) {
			bShowMouseCursor = true;
		}
	}
}

void ACPlayerController::SetPush(bool b)
{
	if (PlayerUIWidget) {
		PlayerUIWidget->setPushVisibility(b);
	}
}

void ACPlayerController::SetTake(bool b, FText text)
{
	if (PlayerUIWidget) {
		PlayerUIWidget->setTakeVisibility(b, text);
	}
}


void ACPlayerController::SetPlayerUIWG(UCPlayerUIWidget *w)
{
	PlayerUIWidget = w;
	if (PlayerUIWidget)
		PlayerUIWidget->AddToViewport();
}

void ACPlayerController::TogglePauseMenu()
{
	if (!IsPaused()) {
		bShowMouseCursor = true; 
		FInputModeGameAndUI Mode;
		SetInputMode(Mode);
		PlayerUIWidget->SetPauseMenu(true);
		SetPause(true);
	}
	else {
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		PlayerUIWidget->SetPauseMenu(false);
		SetPause(false);
	}
}

