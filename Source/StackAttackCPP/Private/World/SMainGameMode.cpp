// Fill out your copyright notice in the Description page of Project Settings.


#include "SMainGameMode.h"
#include "SRoom.h"
#include "SPlayerController.h"
#include "SPlayerState.h"
#include "SPlayerUIWidget.h"
#include "SSaveLoadManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ASMainGameMode::ASMainGameMode() {
	
	ConstructorHelpers::FClassFinder<USPlayerUIWidget> wg(TEXT("/Game/UI/PlayerUI.PlayerUI_C"));
	if (wg.Succeeded()) {
		playerUI = CreateWidget< USPlayerUIWidget>(GetWorld(), wg.Class);
	}
}


void ASMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	//Set player widget
	GetWorld()->GetFirstPlayerController<ASPlayerController>()->SetPlayerUIWG(playerUI);

	//Spawn save/load manager
	SLManager = GetWorld()->SpawnActor<ASSaveLoadManager>(FVector(0), FRotator::ZeroRotator);
	if (SLManager) {
		playerUI->SLManager = SLManager;
	}

	//if loading
	if (FCString::Atoi(*UGameplayStatics::ParseOption(OptionsString, "load"))) {
		SLManager->LoadGame("New");
	}
	else {
		Cast<ASRoom>(UGameplayStatics::GetActorOfClass(GetWorld(), TSubclassOf<ASRoom>(ASRoom::StaticClass())))
			->InitializeRoom(FCString::Atoi(*UGameplayStatics::ParseOption(OptionsString, "rW")));
	}	
	GetWorld()->GetFirstPlayerController<ASPlayerController>()->InitController();
	
}
