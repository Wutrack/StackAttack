// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FItemDestroyed.generated.h"

/**
 * 
 */
USTRUCT()
struct FItemDestroyed
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
	int32 boxCount;
		UPROPERTY(EditAnywhere)
	int32 rockCount;

		UPROPERTY(EditAnywhere)
	int32 points;

		UPROPERTY(EditAnywhere)
	bool bEmpty;

	void AddItem(class AItem*);
	void Clear();

	friend void operator+(FItemDestroyed&, const FItemDestroyed&);
};
