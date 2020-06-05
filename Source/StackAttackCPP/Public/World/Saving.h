// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Item.h"
#include "Crane.h"
#include "Room.h"
#include "MainCharacter.h"
#include "FItemDestroyed.h"
#include "MainPlayerState.h"
#include "Saving.generated.h"

/**
 * 
 */


UCLASS()
class STACKATTACKCPP_API USaving : public USaveGame
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
        TArray<FSaveItemStruct> ItemStructArr;
    UPROPERTY(EditAnywhere)
        TArray<FSaveCraneStruct> CraneStructArr;
    UPROPERTY(EditAnywhere)
        FSaveCharacter CharacterStruct;
    UPROPERTY(EditAnywhere)
        FItemDestroyed PSStruct;
    UPROPERTY(EditAnywhere)
        FSaveRoomStruct RoomStruct;

public:

    USaving() {};

    void AddItem(const FSaveItemStruct &save) { ItemStructArr.Add(save); };
    void AddCrane(const FSaveCraneStruct &save) { CraneStructArr.Add(save); };
    void SaveCharacter(const FSaveCharacter &save) { CharacterStruct = save; };
    void SavePlayerState(const FItemDestroyed &save) { PSStruct = save; };
    void SaveRoomStruct(const FSaveRoomStruct &save) { RoomStruct = save; };

    const TArray<FSaveItemStruct> &GetItemStructArr() const { return ItemStructArr; };
    const TArray<FSaveCraneStruct> &GetCraneStructArr() const { return CraneStructArr; };
    const FSaveCharacter &GetCharacterStruct() const { return CharacterStruct; };
    const FItemDestroyed &GetPlayerStateStruct() const { return PSStruct; };
    const FSaveRoomStruct &GetRoomStruct() const { return RoomStruct; };

};
