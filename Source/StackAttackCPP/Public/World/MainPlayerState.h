// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FItemDestroyed.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

/**
 * 
 */

UCLASS()
class STACKATTACKCPP_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()

	FItemDestroyed ItemStruct;	

public:
	AMainPlayerState() {
		ItemStruct.Clear();
	}

	const FItemDestroyed &GetItemStruct() const { return ItemStruct; };
	const FItemDestroyed& UpdateStruct(const FItemDestroyed &);

	const FItemDestroyed &SavePlayerState() const;
	void LoadPlayerState(const FItemDestroyed &);
};
