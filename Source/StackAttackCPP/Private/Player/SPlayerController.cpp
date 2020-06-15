// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "SPlayerState.h"
#include "SCharacter.h"
#include "SPlayerUIWidget.h"
#include "SMainMenu.h"
#include "SFloor.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


ASPlayerController::ASPlayerController()
{
}

void ASPlayerController::BeginPlay() {

	Super::BeginPlay();

	PlayerState = GetPlayerState<ASPlayerState>();
	Character = Cast<ASCharacter>(GetCharacter());

	SetInputMode(FInputModeGameOnly());
}

void ASPlayerController::InitController() {

	//Add destroy line delegate
	Cast<ASFloor>(UGameplayStatics::GetActorOfClass(GetWorld(), TSubclassOf<ASFloor>(ASFloor::StaticClass())))
		->OnItemDestroy.AddDynamic(this, &ASPlayerController::UpdateStruct);

	GetWorld()->GetTimerManager().SetTimer(UpdateUITimer, this, &ASPlayerController::UpdateUI, 0.2, false);
}

void ASPlayerController::UpdateUI()
{
	if (PlayerUIWidget && PlayerState && Character) {
		UpdatePoints(0);
		UpdateHealth(Character->GetHealth());
	}
}

void ASPlayerController::UpdateStruct(const FSItemDestroyedCounter &str)
{
	if (PlayerState) {
		const FSItemDestroyedCounter &temp = PlayerState->UpdateStruct(str);
		PlayerUIWidget->UpdatePoints(temp.points);
	}
}


void ASPlayerController::UpdatePoints(int points)
{
	if (PlayerUIWidget && PlayerState) {
		int32 temp = PlayerState->UpdatePoints(points);
		PlayerUIWidget->UpdatePoints(temp);
	}
}
void ASPlayerController::UpdateCoin(int coin)
{
	if (PlayerUIWidget && PlayerState) {
		int32 temp = PlayerState->UpdateCoin(coin);
		PlayerUIWidget->UpdateCoin(temp);
	}
}

void ASPlayerController::UpdateHealth(int health)
{
	if (PlayerUIWidget) {
		PlayerUIWidget->updateHealth(health);
		if (health == 0) {
			bShowMouseCursor = true;
		}
	}
}

void ASPlayerController::SetPush(bool b)
{
	if (PlayerUIWidget) {
		PlayerUIWidget->setPushVisibility(b);
	}
}

void ASPlayerController::SetTake(bool b, FText text)
{
	if (PlayerUIWidget) {
		PlayerUIWidget->setTakeVisibility(b, text);
	}
}


void ASPlayerController::SetPlayerUIWG(USPlayerUIWidget *w)
{
	PlayerUIWidget = w;
	if (PlayerUIWidget)
		PlayerUIWidget->AddToViewport();
}

void ASPlayerController::TogglePauseMenu()
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

