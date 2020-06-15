// Fill out your copyright notice in the Description page of Project Settings.


#include "SMainMenuGameMode.h"
#include "SPlayerController.h"
#include "SMainMenu.h"
#include "SRoom.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"


void ASMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	FInputModeUIOnly mode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(mode);

}

ASMainMenuGameMode::ASMainMenuGameMode()
{
	ConstructorHelpers::FClassFinder<USMainMenu> wg(TEXT("/Game/UI/MainMenuUI"));
	WGClass = wg.Class;

	MainMenuWidget = CreateWidget< USMainMenu>(GetWorld(), WGClass);
	if (MainMenuWidget)
		MainMenuWidget->AddToViewport();
}
