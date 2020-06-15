// Fill out your copyright notice in the Description page of Project Settings.


#include "FSItemDestroyedCounter.h"
#include "Engine/Engine.h"
#include "SItemBase.h"

void FSItemDestroyedCounter::AddItem(ASItemBase *item)
{
	switch (item->GetType())
	{
	case EItemType::Box:
		boxCount++;
	case EItemType::Rock:
		rockCount++;
	}
	points += item->GetPoints();

}

void FSItemDestroyedCounter::Clear()
{
	bEmpty		= true;
	boxCount	= 0;
	rockCount	= 0;
	points		= 0;
}

void operator+(FSItemDestroyedCounter & str1, const FSItemDestroyedCounter & str2)
{
	str1.boxCount += str2.boxCount;
	str1.rockCount += str2.rockCount;
	str1.points += str2.points;
}
