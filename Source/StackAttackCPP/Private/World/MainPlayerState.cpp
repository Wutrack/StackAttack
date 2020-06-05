// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerState.h"


const FItemDestroyed &AMainPlayerState::UpdateStruct(const FItemDestroyed &str)
{
	ItemStruct + str;
	return ItemStruct;
}

const FItemDestroyed &AMainPlayerState::SavePlayerState() const
{
	return ItemStruct; 
}

void AMainPlayerState::LoadPlayerState(const FItemDestroyed &str)
{
	ItemStruct = str;
}
