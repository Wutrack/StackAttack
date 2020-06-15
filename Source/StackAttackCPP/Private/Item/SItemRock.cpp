// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemRock.h"
#include "Engine/Engine.h"

ASItemRock::ASItemRock() {

	ItemType = EItemType::Rock;
	Points = 15;
	HitDamage = 2;
}