// Fill out your copyright notice in the Description page of Project Settings.


#include "FItemDestroyed.h"
#include "Engine/Engine.h"
#include "Item.h"

void FItemDestroyed::AddItem(AItem *item)
{
	switch (item->GetType())
	{
	case EItemType::Box:
		boxCount++;
	case EItemType::Rock:
		rockCount++;
	}
	points += item->GetPoints();

	bEmpty = false;
}

void FItemDestroyed::Clear()
{
	bEmpty = true;
	boxCount = 0;
	rockCount = 0;
	points = 0;
}

void operator+(FItemDestroyed & str1, const FItemDestroyed & str2)
{
	str1.boxCount += str2.boxCount;
	str1.rockCount += str2.rockCount;
	str1.points += str2.points;
}
