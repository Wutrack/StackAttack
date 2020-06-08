 // Fill out your copyright notice in the Description page of Project Settings.

#include "CMainMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"	
#include "Kismet/GameplayStatics.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Internationalization/Regex.h"


bool UCMainMenu::Initialize()
{
    bool Success = Super::Initialize();

    if (!Success) return false;

    SlotName = "New"; 
    SoloLevel = "Solo_Game_level";


    if (!ensure(MainMenuWS)) { return false; } // check for nullptrs

    //Main menu 0
    if (!ensure(StartGameBt)) { return false; }
    if (!ensure(OptionsBt)) { return false; } 
    if (!ensure(ExitGameBt)) { return false; } 

    //Game menu 1
    if (!ensure(NewGameBt)) { return false; }
    if (!ensure(LoadGameBt)) { return false; }
    if (!ensure(BackToMainMenuBt)) { return false; }

    //Set map menu 2
    if (!ensure(SetDefMapBt)) { return false; }
    if (!ensure(SetCustomMapBt)) { return false; }
    if (!ensure(BackToGameMenuBt)) { return false; }
     
    //Start custom map menu 3
    if (!ensure(StartCustomMapBt)) { return false; }
    if (!ensure(BackToSetMapMenuBt)) { return false; }


    //Main menu 0
    StartGameBt->OnClicked.AddDynamic(this, &UCMainMenu::StartNewGame);
    OptionsBt->OnClicked.AddDynamic(this, &UCMainMenu::Options);
    ExitGameBt->OnClicked.AddDynamic(this, &UCMainMenu::Exit);

    //Game menu 1
    NewGameBt->OnClicked.AddDynamic(this, &UCMainMenu::NewGame);
    LoadGameBt->OnClicked.AddDynamic(this, &UCMainMenu::LoadGame);
    BackToMainMenuBt->OnClicked.AddDynamic(this, &UCMainMenu::BackToMenu);

    //Set map menu 2
    SetDefMapBt->OnClicked.AddDynamic(this, &UCMainMenu::PlayDefMap);
    SetCustomMapBt->OnClicked.AddDynamic(this, &UCMainMenu::SetCustomMap);
    BackToGameMenuBt->OnClicked.AddDynamic(this, &UCMainMenu::BackToGameMenu);

    //Start custom map menu 3
    rWidthInput->SelectAllTextWhenFocused = true;
    StartCustomMapBt->OnClicked.AddDynamic(this, &UCMainMenu::StartCustomMap);
    BackToSetMapMenuBt->OnClicked.AddDynamic(this, &UCMainMenu::BackToSetMapMenu);

    return true;
}

//Info block for error and information
void UCMainMenu::SetInfoText(FString str)
{
    InfoText->SetText(FText::FromString(str));
    GetWorld()->GetTimerManager().SetTimer(InfoHandle, this, &UCMainMenu::ClearInfoText, 2, false, 2);
}

void UCMainMenu::ClearInfoText()
{
    InfoText->SetText(FText());
}


//Main menu 0
void UCMainMenu::StartNewGame()
{
    MainMenuWS->SetActiveWidgetIndex(1);
}

void UCMainMenu::Options()
{
    GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Purple, "Options in progress");
}

void UCMainMenu::Exit()
{
    GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}

//Game menu 1
void UCMainMenu::NewGame()
{
    MainMenuWS->SetActiveWidgetIndex(2);
}

void UCMainMenu::LoadGame()
{
    if (UGameplayStatics::DoesSaveGameExist(SlotName, 0)) {
        OpenLevel(SoloLevel, "load=1", true);
    }
    else {
        SetInfoText("No saved game!");
    }
}

void UCMainMenu::BackToMenu()
{
    MainMenuWS->SetActiveWidgetIndex(0);
}

//Set map menu 2
void UCMainMenu::PlayDefMap()
{
    OpenLevel(SoloLevel, "rW=10?load=0", true);
}

void UCMainMenu::SetCustomMap()
{
    MainMenuWS->SetActiveWidgetIndex(3);
}

void UCMainMenu::BackToGameMenu()
{
    MainMenuWS->SetActiveWidgetIndex(1);
}

//Start custom map menu 3
void UCMainMenu::StartCustomMap()
{
    if (rWidthInput->Text.ToString() != "") {

            FString Option = "rW=" + rWidthInput->Text.ToString() + "?load=0";

            OpenLevel(SoloLevel, Option, true);
    }
    else {
        SetInfoText("Room width cann`t be empty!\nEnter number between 2-50");
    }
}

void UCMainMenu::BackToSetMapMenu()
{
    MainMenuWS->SetActiveWidgetIndex(2);
}



void UCMainMenu::OpenLevel(FName level, FString option, bool b)
{
    UGameplayStatics::OpenLevel(this, level, b, option);
}