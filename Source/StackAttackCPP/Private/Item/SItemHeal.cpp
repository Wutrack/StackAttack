// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemHeal.h"

ASItemHeal::ASItemHeal() {

	bCanPickUpItem = true;
	BoostPower = 1;
	Points = 20;

	ItemType = EItemType::Boost;
	BoostType = EBoostType::Heal;
}