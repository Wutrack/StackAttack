// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemRock.generated.h"

/**
 * 
 */
UCLASS()
class STACKATTACKCPP_API AItemRock : public AItem
{
	GENERATED_BODY()


public:
	AItemRock();

	virtual bool CanPush(const FVector v) override {
		return false;
	};

protected:


}; 
