// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerUIWidget.h"
#include "SPlayerController.h"
#include "SSaveLoadManager.h"
#include "SFloor.h"
#include "Engine/Engine.h"
#include "Components/TextBlock.h"	
#include "Components/VerticalBox.h"	
#include "Components/HorizontalBox.h"	
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"


bool USPlayerUIWidget::Initialize() {

	bool Success = Super::Initialize();

	GameOverBox->SetVisibility(ESlateVisibility::Hidden);

    if (!ensure(ToMainMenuBt)) { return false; }
    if (!ensure(ResumeGameBt)) { return false; }
    if (!ensure(SaveGameBt)) { return false; }
    if (!ensure(BackToMainMenuBt)) { return false; }

	ToMainMenuBt->OnClicked.AddDynamic(this, &USPlayerUIWidget::BackToMainMenu);
	ResumeGameBt->OnClicked.AddDynamic(this, &USPlayerUIWidget::Unpause);
	SaveGameBt->OnClicked.AddDynamic(this, &USPlayerUIWidget::SaveGame);
	BackToMainMenuBt->OnClicked.AddDynamic(this, &USPlayerUIWidget::BackToMainMenu);

	return Success;
}

void USPlayerUIWidget::BackToMainMenu()
{
	UGameplayStatics::OpenLevel(this, "Main_Menu", true); 
	Unpause();
}

void USPlayerUIWidget::Unpause()
{
	Cast<ASPlayerController>(GetWorld()->GetFirstPlayerController())->TogglePauseMenu();
}

void USPlayerUIWidget::SaveGame()
{
	if (SLManager != nullptr) {
		SLManager->SaveGame();
	}
}

void USPlayerUIWidget::UpdatePoints(int32 points)
{
	PointsText->SetText(FText::AsNumber(points));
}

void USPlayerUIWidget::UpdateCoin(int32 points)
{
	CoinText->SetText(FText::AsNumber(points));
}

void USPlayerUIWidget::updateHealth(int32 hp)
{
	HealthText->SetText(FText::AsNumber(hp));
	if (hp == 0)
	{
		PointsBox->SetVisibility(ESlateVisibility::Hidden);
		GameOverBox->SetVisibility(ESlateVisibility::Visible);
		TotalPointsText->SetText(PointsText->GetText());
	}
}

void USPlayerUIWidget::setPushVisibility(bool b)
{
	if (b) {
		PushText->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		PushText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USPlayerUIWidget::setTakeVisibility(bool b, FText &text)
{
	TakeText->SetText(text);
	if (b) {
		TakeBox->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		TakeBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USPlayerUIWidget::SetPauseMenu(bool b)
{
	if (b) {
		PauseMenuBox->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		PauseMenuBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

