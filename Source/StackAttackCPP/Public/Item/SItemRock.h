// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SItemBase.h"
#include "SItemRock.generated.h"

/**
 * 
 */
UCLASS()
class STACKATTACKCPP_API ASItemRock : public ASItemBase
{
	GENERATED_BODY()


public:
	ASItemRock();

	virtual bool CanPush(const FVector v) override {
		return false;
	};

protected:


}; 
