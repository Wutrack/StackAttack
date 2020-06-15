// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSItemDestroyedCounter.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

/**
 * 
 */
USTRUCT()
struct FPlayerStateStruct {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
		int32						CoinCount;
	UPROPERTY(EditAnywhere)
		FSItemDestroyedCounter		ItemStruct;
};


UCLASS()
class STACKATTACKCPP_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
		
	FPlayerStateStruct				PlayerStateStruct;
	FSItemDestroyedCounter			ItemStruct;
	int32							CoinCount;

public:
	ASPlayerState();

	const FSItemDestroyedCounter &	GetItemStruct() const	 { return ItemStruct; };
	const FSItemDestroyedCounter &	UpdateStruct			 (const FSItemDestroyedCounter &str);

	int32		UpdatePoints		(int32);
	int32		UpdateCoin			(int32);

	const FPlayerStateStruct &		SavePlayerState();
	void							LoadPlayerState			 (const FPlayerStateStruct &);

};
