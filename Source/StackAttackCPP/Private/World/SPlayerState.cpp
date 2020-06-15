// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "Engine/Engine.h"


ASPlayerState::ASPlayerState() {

	ItemStruct.Clear();
	CoinCount = 0;
}

const FSItemDestroyedCounter &ASPlayerState::UpdateStruct(const FSItemDestroyedCounter &str)
{
	ItemStruct + str;
	return ItemStruct;
}

int32 ASPlayerState::UpdatePoints(int32 points)
{
	ItemStruct.points += points;
	return ItemStruct.points;
}

int32 ASPlayerState::UpdateCoin(int32 coin)
{
	CoinCount += coin;
	return CoinCount;
}

const FPlayerStateStruct &ASPlayerState::SavePlayerState()
{
	PlayerStateStruct.CoinCount = CoinCount;
	PlayerStateStruct.ItemStruct = ItemStruct;

	return PlayerStateStruct;
}

void ASPlayerState::LoadPlayerState(const FPlayerStateStruct &str)
{
	ItemStruct = str.ItemStruct;
	CoinCount = str.CoinCount;
}

