// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSItemDestroyedCounter.generated.h"

/**
 * 
 */
USTRUCT()
struct FSItemDestroyedCounter
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	int32					boxCount	= 0;
	UPROPERTY(EditAnywhere)
	int32					rockCount	= 0;

	UPROPERTY(EditAnywhere)
	int32					points		= 0;

	UPROPERTY(EditAnywhere)
	bool					bEmpty		= true;


	void		AddItem		(class ASItemBase*);
	void		Clear();

	friend void operator+	(FSItemDestroyedCounter &, const FSItemDestroyedCounter &);
};
