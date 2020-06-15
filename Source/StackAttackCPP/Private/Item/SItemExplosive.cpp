// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemExplosive.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

ASItemExplosive::ASItemExplosive() {

	ItemType = EItemType::Explosive;
	BoostType = EBoostType::None;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->AttachToComponent(ItemMesh, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionSphere->SetSphereRadius(100);
	CollisionSphere->SetCollisionProfileName("ExplosiveSphere");
}

void ASItemExplosive::BeginPlay()
{
	ASItemBase::BeginPlay();

	//Set active Material
	ActiveMaterial = SpecialMaterialCollection[rand() % SpecialMaterialCollection.Num()];
}

void ASItemExplosive::StopPush()
{
	ASItemBase::StopPush();
	RollToActive();
}

void ASItemExplosive::StopFalling()
{
	ASItemBase::StopFalling();
	RollToActive();
}

void ASItemExplosive::RollToActive()
{
	if (!bNormalActive && !bExplodeActive) {
		if (rand() % 2) {
			SetNormalActiveState();
		}
	}
}

void ASItemExplosive::SetNormalActiveState()
{
	bNormalActive = true;
	SetActiveChange();
	GetWorld()->GetTimerManager().SetTimer(ExplodeHandle, this, &ASItemExplosive::DestroyItem, 0.1, false, 5.0f);
}

void ASItemExplosive::SetExplodeActiveState()
{
	bNormalActive = false;
	bExplodeActive = true; 
	SetActiveChange();
	GetWorld()->GetTimerManager().SetTimer(ExplodeHandle, this, &ASItemExplosive::DestroyItem, 0.1, false, .5f);
}
void ASItemExplosive::SetActiveChange()
{
	ItemMesh->SetMaterial(0, ActiveMaterial);
	CollisionSphere->SetGenerateOverlapEvents(true);
}

void ASItemExplosive::Explode()
{
	bExplode = true;
	TArray<AActor *> arr;
	CollisionSphere->GetOverlappingActors(arr, TSubclassOf<ASItemBase>());
	
	//Loop all items nearby
	for (auto i : arr) {
		if (IsValid(i) && i != this && Cast<ASItemBase>(i)) {
			if (Cast<ASItemExplosive>(i)) {
				ASItemExplosive* expl = Cast<ASItemExplosive>(i);

				if (!expl->bExplode) {
					if (!expl->bNormalActive && !expl->bExplodeActive) {
						expl->SetExplodeActiveState();
					}
					else if (expl->bNormalActive){
						GetWorld()->GetTimerManager().ClearTimer(expl->ExplodeHandle);
						expl->SetExplodeActiveState();
					}
					else {
						GetWorld()->GetTimerManager().ClearTimer(expl->ExplodeHandle);
						Cast<ASItemBase>(i)->DestroyItem();
					}
				}
			}
			else {
				Cast<ASItemBase>(i)->DestroyItem();
			}
		}
	}
}

void ASItemExplosive::DestroyItem()
{
	Explode();

	ASItemBase::DestroyItem();
}


FSaveItemStruct ASItemExplosive::SaveItem()
{
	FSaveItemStruct str = ASItemBase::SaveItem();

	str.AdditionalBoolValue.Add(bExplode);
	str.AdditionalBoolValue.Add(bNormalActive);
	str.AdditionalBoolValue.Add(bExplodeActive);

	return str;
}

ASItemBase *ASItemExplosive::LoadItem(const FSaveItemStruct &load)
{
	ASItemBase::LoadItem(load);

	bExplode = load.AdditionalBoolValue[0];
	bNormalActive = load.AdditionalBoolValue[1];
	bExplodeActive = load.AdditionalBoolValue[2];
	
	//Set active if it was
	if (bNormalActive)
	{
		SetActiveChange();
		GetWorld()->GetTimerManager().SetTimer(ExplodeHandle, this, &ASItemExplosive::DestroyItem, 0.1, false, 4.0f);
	}
	else if (bExplodeActive || bExplode)
	{
		SetActiveChange();
		DestroyItem();
	}
	
	return this;
}