// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerUIWidget.h"
#include "Engine/Engine.h"
#include "Components/TextBlock.h"	
#include "Components/VerticalBox.h"	
#include "Components/HorizontalBox.h"	
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "CPlayerController.h"
#include "SaveLoadManager.h"


bool UCPlayerUIWidget::Initialize() {

	bool Success = Super::Initialize();

	GameOverBox->SetVisibility(ESlateVisibility::Hidden);

    if (!ensure(ToMainMenuBt)) { return false; }
    if (!ensure(ResumeGameBt)) { return false; }
    if (!ensure(SaveGameBt)) { return false; }
    if (!ensure(BackToMainMenuBt)) { return false; }

	ToMainMenuBt->OnClicked.AddDynamic(this, &UCPlayerUIWidget::BackToMainMenu);
	ResumeGameBt->OnClicked.AddDynamic(this, &UCPlayerUIWidget::Unpause);
	SaveGameBt->OnClicked.AddDynamic(this, &UCPlayerUIWidget::SaveGame);
	BackToMainMenuBt->OnClicked.AddDynamic(this, &UCPlayerUIWidget::BackToMainMenu);

	return Success;
}

void UCPlayerUIWidget::BackToMainMenu()
{
	UGameplayStatics::OpenLevel(this, "Main_Menu", true); 
	Unpause();
}

void UCPlayerUIWidget::Unpause()
{
	Cast<ACPlayerController>(GetWorld()->GetFirstPlayerController())->TogglePauseMenu();
}

void UCPlayerUIWidget::SaveGame()
{
	if (SLManager != nullptr) {
		SLManager->SaveGame();
	}
}

void UCPlayerUIWidget::updatePoints(int32 points)
{
	PointsText->SetText(FText::AsNumber(points));
}

void UCPlayerUIWidget::updateHealth(int32 hp)
{
	HealthText->SetText(FText::AsNumber(hp));
	if (hp == 0)
	{
		PointsBox->SetVisibility(ESlateVisibility::Hidden);
		GameOverBox->SetVisibility(ESlateVisibility::Visible);
		TotalPointsText->SetText(PointsText->GetText());
	}
}

void UCPlayerUIWidget::setPushVisibility(bool b)
{
	if (b) {
		PushText->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		PushText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCPlayerUIWidget::setTakeVisibility(bool b, FText &text)
{
	TakeText->SetText(text);
	if (b) {
		TakeBox->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		TakeBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCPlayerUIWidget::SetPauseMenu(bool b)
{
	if (b) {
		PauseMenuBox->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		PauseMenuBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

