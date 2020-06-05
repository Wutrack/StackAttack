// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "CPlayerController.h"
#include "CMainMenu.h"
#include "Room.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"


void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	FInputModeUIOnly mode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(mode);

}

AMainMenuGameMode::AMainMenuGameMode()
{
	ConstructorHelpers::FClassFinder<UCMainMenu> wg(TEXT("/Game/UI/MainMenuUI"));
	WGClass = wg.Class;

	MainMenuWidget = CreateWidget< UCMainMenu>(GetWorld(), WGClass);
	if (MainMenuWidget)
		MainMenuWidget->AddToViewport();
}
