 // Fill out your copyright notice in the Description page of Project Settings.

#include "SMainMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"	
#include "Kismet/GameplayStatics.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Internationalization/Regex.h"


bool USMainMenu::Initialize()
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
    StartGameBt->OnClicked.AddDynamic(this, &USMainMenu::StartNewGame);
    OptionsBt->OnClicked.AddDynamic(this, &USMainMenu::Options);
    ExitGameBt->OnClicked.AddDynamic(this, &USMainMenu::Exit);

    //Game menu 1
    NewGameBt->OnClicked.AddDynamic(this, &USMainMenu::NewGame);
    LoadGameBt->OnClicked.AddDynamic(this, &USMainMenu::LoadGame);
    BackToMainMenuBt->OnClicked.AddDynamic(this, &USMainMenu::BackToMenu);

    //Set map menu 2
    SetDefMapBt->OnClicked.AddDynamic(this, &USMainMenu::PlayDefMap);
    SetCustomMapBt->OnClicked.AddDynamic(this, &USMainMenu::SetCustomMap);
    BackToGameMenuBt->OnClicked.AddDynamic(this, &USMainMenu::BackToGameMenu);

    //Start custom map menu 3
    rWidthInput->SelectAllTextWhenFocused = true;
    StartCustomMapBt->OnClicked.AddDynamic(this, &USMainMenu::StartCustomMap);
    BackToSetMapMenuBt->OnClicked.AddDynamic(this, &USMainMenu::BackToSetMapMenu);

    return true;
}

//Info block for error and information
void USMainMenu::SetInfoText(FString str)
{
    InfoText->SetText(FText::FromString(str));
    GetWorld()->GetTimerManager().SetTimer(InfoHandle, this, &USMainMenu::ClearInfoText, 2, false, 2);
}

void USMainMenu::ClearInfoText()
{
    InfoText->SetText(FText());
}


//Main menu 0
void USMainMenu::StartNewGame()
{
    MainMenuWS->SetActiveWidgetIndex(1);
}

void USMainMenu::Options()
{
    GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Purple, "Options in progress");
}

void USMainMenu::Exit()
{
    GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}

//Game menu 1
void USMainMenu::NewGame()
{
    MainMenuWS->SetActiveWidgetIndex(2);
}

void USMainMenu::LoadGame()
{
    if (UGameplayStatics::DoesSaveGameExist(SlotName, 0)) {
        OpenLevel(SoloLevel, "load=1", true);
    }
    else {
        SetInfoText("No saved game!");
    }
}

void USMainMenu::BackToMenu()
{
    MainMenuWS->SetActiveWidgetIndex(0);
}

//Set map menu 2
void USMainMenu::PlayDefMap()
{
    OpenLevel(SoloLevel, "rW=10?load=0", true);
}

void USMainMenu::SetCustomMap()
{
    MainMenuWS->SetActiveWidgetIndex(3);
}

void USMainMenu::BackToGameMenu()
{
    MainMenuWS->SetActiveWidgetIndex(1);
}

//Start custom map menu 3
void USMainMenu::StartCustomMap()
{
    if (rWidthInput->Text.ToString() != "") {
        if (FCString::Atoi(*rWidthInput->Text.ToString()) > 2) {

            FString Option = "rW=" + rWidthInput->Text.ToString() + "?load=0";

            OpenLevel(SoloLevel, Option, true);
        }
        else {
            rWidthInput->SetText(FText::AsCultureInvariant("3"));
            SetInfoText("Room width Cant be less then 3!");
        }

    }
    else {
        rWidthInput->SetText(FText::AsCultureInvariant("3"));
        SetInfoText("Room width cann`t be empty!\nEnter number between 3-50");
    }
}

void USMainMenu::BackToSetMapMenu()
{
    MainMenuWS->SetActiveWidgetIndex(2);
}



void USMainMenu::OpenLevel(FName level, FString option, bool b)
{
    UGameplayStatics::OpenLevel(this, level, b, option);
}