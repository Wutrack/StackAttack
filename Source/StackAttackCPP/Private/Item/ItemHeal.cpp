// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemHeal.h"

AItemHeal::AItemHeal() {

	bTake = true;
	BoostPower = 1;
	Points = 20;

	Type = EItemType::Boost;
	BoostType = EBoostType::Heal;
}