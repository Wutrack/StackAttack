// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemExplosive.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

AItemExplosive::AItemExplosive() {

	Type = EItemType::Explosive;
	BoostType = EBoostType::None;
	bExplode = false; 
	bActive = false;
	bExplodeActive = false;	

	SphereColl = CreateDefaultSubobject<USphereComponent>(TEXT("SphereColl"));
	SphereColl->AttachToComponent(ItemMesh, FAttachmentTransformRules::KeepRelativeTransform);
	SphereColl->SetSphereRadius(100);
	SphereColl->SetCollisionProfileName("Explosive");
}

void AItemExplosive::BeginPlay()
{
	AItem::BeginPlay();

	//Set active Material
	ActiveMaterial = MaterialArrSpecialBP[rand() % MaterialArrSpecialBP.Num()];
}

void AItemExplosive::StopPush()
{
	AItem::StopPush();
	RollToActive();
}

void AItemExplosive::StopFalling()
{
	AItem::StopFalling();
	RollToActive();
}

void AItemExplosive::RollToActive()
{
	if (!bActive && !bExplodeActive) {
		if (rand() % 2) {
			SetActive();
		}
	}
}

void AItemExplosive::SetActive()
{
	bActive = true; 
	SetActiveChange();
	GetWorld()->GetTimerManager().SetTimer(ExplodeHandle, this, &AItemExplosive::DestroyItem, 0.1, false, 5.0f);
}

void AItemExplosive::SetExplodeActive()
{
	bActive = false;
	bExplodeActive = true; 
	SetActiveChange();
	GetWorld()->GetTimerManager().SetTimer(ExplodeHandle, this, &AItemExplosive::DestroyItem, 0.1, false, .5f);
}
void AItemExplosive::SetActiveChange()
{
	ItemMesh->SetMaterial(0, ActiveMaterial);
	SphereColl->SetGenerateOverlapEvents(true);
}

void AItemExplosive::Explode()
{
	bExplode = true;
	TArray<AActor*> arr;
	SphereColl->GetOverlappingActors(arr, TSubclassOf<AItem>());
	
	//Loop all items nearby
	for (auto i : arr) {
		if (IsValid(i) && i != this && Cast<AItem>(i)) {
			if (Cast<AItemExplosive>(i)) {
				AItemExplosive* expl = Cast<AItemExplosive>(i);

				if (!expl->bExplode) {
					if (!expl->bActive && !expl->bExplodeActive) {
						expl->SetExplodeActive();
					}
					else if(expl->bActive){
						GetWorld()->GetTimerManager().ClearTimer(expl->ExplodeHandle);
						expl->SetExplodeActive();
					}
					else {
						GetWorld()->GetTimerManager().ClearTimer(expl->ExplodeHandle);
						Cast<AItem>(i)->DestroyItem();
					}
				}
			}
			else {
				Cast<AItem>(i)->DestroyItem();
			}
		}
	}
}

void AItemExplosive::DestroyItem()
{
	Explode();

	AItem::DestroyItem();
}


FSaveItemStruct AItemExplosive::SaveItem()
{
	FSaveItemStruct str = AItem::SaveItem();

	str.AdditionalBool.Add(bExplode);
	str.AdditionalBool.Add(bActive);
	str.AdditionalBool.Add(bExplodeActive);

	return str;
}

AItem *AItemExplosive::LoadItem(const FSaveItemStruct &load)
{
	AItem::LoadItem(load);

	bExplode = load.AdditionalBool[0];
	bActive = load.AdditionalBool[1];
	bExplodeActive = load.AdditionalBool[2];
	
	//Set active if it was
	if (bActive)
	{
		SetActiveChange();
		GetWorld()->GetTimerManager().SetTimer(ExplodeHandle, this, &AItemExplosive::DestroyItem, 0.1, false, 4.0f);
	}
	else if (bExplodeActive || bExplode)
	{
		SetActiveChange();
		DestroyItem();
	}
	
	return this;
}