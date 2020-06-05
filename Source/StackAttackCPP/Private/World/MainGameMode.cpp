// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "Room.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "CPlayerController.h"
#include "CPlayerUIWidget.h"
#include "SaveLoadManager.h"
#include <cassert>

AMainGameMode::AMainGameMode() {

	ConstructorHelpers::FClassFinder<UCPlayerUIWidget> wg(TEXT("/Game/UI/PlayerUI.PlayerUI_C"));
	if (wg.Succeeded()) {
		playerUI = CreateWidget< UCPlayerUIWidget>(GetWorld(), wg.Class);
	}
}


void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	//Set player widget
	GetWorld()->GetFirstPlayerController<ACPlayerController>()->SetPlayerUIWG(playerUI);

	//Spawn save/load manager
	SLManager = GetWorld()->SpawnActor<ASaveLoadManager>(FVector(0), FRotator::ZeroRotator);
	if (SLManager) {
		playerUI->SLManager = SLManager;
	}

	//if loading
	if (FCString::Atoi(*UGameplayStatics::ParseOption(OptionsString, "load"))) {
		SLManager->LoadGame("New");
	}
	else {
		Cast<ARoom>(UGameplayStatics::GetActorOfClass(GetWorld(), TSubclassOf<ARoom>(ARoom::StaticClass())))
			->InitRoom(FCString::Atoi(*UGameplayStatics::ParseOption(OptionsString, "rW")));
	}
}
