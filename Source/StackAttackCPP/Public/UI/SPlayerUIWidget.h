// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSItemDestroyedCounter.h"
#include "Blueprint/UserWidget.h"
#include "SPlayerUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class STACKATTACKCPP_API USPlayerUIWidget : public UUserWidget
{
    GENERATED_BODY()


    UPROPERTY(meta = (BindWidget))
    class UTextBlock                *HealthText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock                *PointsText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock                *CoinText;

    //Interact menu
    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox            *TakeBox;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock                *PushText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock                *TakeText;


    //Pause menu
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox              *PauseMenuBox;

    UPROPERTY(meta = (BindWidget))
    class UButton                   *ResumeGameBt;

    UPROPERTY(meta = (BindWidget))
    class UButton                   *SaveGameBt;

    UPROPERTY(meta = (BindWidget))
    class UButton                   *BackToMainMenuBt;


    UPROPERTY(meta = (BindWidget))
    class UVerticalBox              *PointsBox;

    //Game over menu
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox              *GameOverBox;


    UPROPERTY(meta = (BindWidget))
    class UTextBlock                *TotalPointsText;

    UPROPERTY(meta = (BindWidget))
    class UButton                   *ToMainMenuBt;
	

    UFUNCTION()
    void        BackToMainMenu();

    UFUNCTION()
    void        Unpause();

    UFUNCTION()
    void        SaveGame();


protected:

    virtual bool Initialize() override; // override this, its like beginplay but for UUserWidgets

public:

    class ASSaveLoadManager             *SLManager;


    void        UpdatePoints            (int32);
    void        UpdateCoin              (int32);

    void        updateHealth            (int32);
    void        setPushVisibility       (bool);
    void        setTakeVisibility       (bool, FText&);

    void        SetPauseMenu            (bool);
};
