// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class STACKATTACKCPP_API UCMainMenu : public UUserWidget
{
    GENERATED_BODY()



    UPROPERTY(meta = (BindWidget))
    class UWidgetSwitcher *MainMenuWS;


    //Main menu 0
    UPROPERTY(meta = (BindWidget))
    class UButton *StartGameBt;

    UPROPERTY(meta = (BindWidget))
    class UButton *OptionsBt;

    UPROPERTY(meta = (BindWidget))
    class UButton *ExitGameBt;

    //Game menu 1
    UPROPERTY(meta = (BindWidget))
    class UButton *NewGameBt;

    UPROPERTY(meta = (BindWidget))
    class UButton *LoadGameBt;

    UPROPERTY(meta = (BindWidget))
    class UButton *BackToMainMenuBt;

    //Set map menu 2
    UPROPERTY(meta = (BindWidget))
    class UButton *SetDefMapBt;

    UPROPERTY(meta = (BindWidget))
    class UButton *SetCustomMapBt;

    UPROPERTY(meta = (BindWidget))
    class UButton *BackToGameMenuBt;

    //Start custom map menu 3
    UPROPERTY(meta = (BindWidget))
    class UButton *StartCustomMapBt;

    UPROPERTY(meta = (BindWidget))
    class UButton *BackToSetMapMenuBt;

    //Info Text
    UPROPERTY(meta = (BindWidget))
    class UTextBlock *InfoText;

    FTimerHandle InfoHandle;

    FString SlotName;
    FName SoloLevel;

    void SetInfoText(FString);
    void ClearInfoText();
    void OpenLevel(FName, FString, bool);

public:

    //Main menu 0
    UFUNCTION()
    void StartNewGame();

    UFUNCTION()
    void Options();

    UFUNCTION()
    void Exit();

    //Game menu 1
    UFUNCTION()
    void NewGame();

    UFUNCTION()
    void LoadGame();

    UFUNCTION()
    void BackToMenu();

    //Set map menu 2
    UFUNCTION()
    void PlayDefMap();

    UFUNCTION()
    void SetCustomMap();

    UFUNCTION()
    void BackToGameMenu();

    //Start custom map menu 3
    UFUNCTION()
    void StartCustomMap();

    UFUNCTION()
    void CheckWhidthInput();

    UFUNCTION()
    void BackToSetMapMenu();

protected:
    virtual bool Initialize() override; // override this, its like beginplay but for UUserWidgets

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        class UEditableTextBox *rWidthInput;

    FText TextRegExpr(FText);
};
