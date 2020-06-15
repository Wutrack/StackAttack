// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SItemBase.h"
#include "SCrane.h"
#include "SRoom.h"
#include "SCoin.h"
#include "SCharacter.h"
#include "FSItemDestroyedCounter.h"
#include "SPlayerState.h"
#include "SSaving.generated.h"

/**
 * 
 */


UCLASS()
class STACKATTACKCPP_API USSaving : public USaveGame
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
        TArray<FSaveItemStruct>     ItemStructArray;
    UPROPERTY(EditAnywhere)
        TArray<FSaveCraneStruct>    CraneStructArray;
    UPROPERTY(EditAnywhere)
        TArray<FCoinSaveStruct>     CoinStructArray;
    UPROPERTY(EditAnywhere)
        FSaveCharacter              CharacterStruct;
    UPROPERTY(EditAnywhere)
        FPlayerStateStruct          PSStruct;
    UPROPERTY(EditAnywhere)
        FSaveRoomStruct             RoomStruct;

public:

    USSaving() {};

    void        AddItem                 (const FSaveItemStruct &save)       { ItemStructArray.Add(save); };
    void        AddCrane                (const FSaveCraneStruct &save)      { CraneStructArray.Add(save); };
    void        AddCoin                 (const FCoinSaveStruct &save)       { CoinStructArray.Add(save); };
    void        SaveCharacter           (const FSaveCharacter &save)        { CharacterStruct = save; };
    void        SavePlayerState         (const FPlayerStateStruct &save)    { PSStruct = save; };
    void        SaveRoomStruct          (const FSaveRoomStruct &save)       { RoomStruct = save; };
    void        SaveCoinsStruct         (const FSaveRoomStruct &save)       { RoomStruct = save; };

    const TArray<FSaveItemStruct> &     GetItemStructArray() const          { return ItemStructArray; };
    const TArray<FSaveCraneStruct> &    GetCraneStructArray() const         { return CraneStructArray; };
    const TArray<FCoinSaveStruct> &     GetCoinStructArray() const          { return CoinStructArray; };
    const FSaveCharacter &              GetCharacterStruct() const          { return CharacterStruct; };
    const FPlayerStateStruct &          GetPlayerStateStruct() const        { return PSStruct; };
    const FSaveRoomStruct &             GetRoomStruct() const               { return RoomStruct; };

};
