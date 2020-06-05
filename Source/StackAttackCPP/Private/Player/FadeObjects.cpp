// Fill out your copyright notice in the Description page of Project Settings.


#include "FadeObjects.h"
#include "Materials/MaterialInterface.h"
#include "Materials/Material.h"
#include "TimerManager.h"
#include "CollisionQueryParams.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UFadeObjects::UFadeObjects()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	checkInterval = 0.1f;
	fadeInterval = 0.1f;

	rateFade = 100.0f;
	targetFade = 0.25f;

	objectTypes.Add(ECC_WorldStatic);
	objectTypes.Add(ECC_WorldDynamic);
	objectTypes.Add(ECC_GameTraceChannel1);

	capsuleHalfHeight = 50.0f;
	capsuleRadius = 30.0f;

	player = GetOwner();
	ConstructorHelpers::FObjectFinder<UMaterial> fo(TEXT("/Game/Item/Material/Special/FadeMat.FadeMat"));
	if (fo.Succeeded()) {
		fadeMat = fo.Object;
	}
}


// Called when the game starts
void UFadeObjects::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(checkObjHandle, this, &UFadeObjects::CheckObjects, checkInterval, true);
	GetWorld()->GetTimerManager().SetTimer(fadeObjHandle, this, &UFadeObjects::FadeObjects, fadeInterval, true);
}


// Called every frame
void UFadeObjects::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFadeObjects::CheckObjects()
{
	const FVector traceStart = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation();
	const FVector traceEnd = player->GetActorLocation();
	const FQuat traceQuat = player->GetActorQuat();

	TArray<FHitResult> hitResults;
	FCollisionQueryParams collQueryParam(TEXT("Coll Params"), true, player);
	TArray<TEnumAsByte<EObjectTypeQuery>> collObjTypes;

	for (int i = 0; i < objectTypes.Num(); i++)
	{
		collObjTypes.Add(UEngineTypes::ConvertToObjectType(objectTypes[i].GetValue()));
	}

	GetWorld()->SweepMultiByObjectType(hitResults, traceStart, traceEnd, traceQuat, collObjTypes,
		FCollisionShape::MakeCapsule(capsuleRadius, capsuleHalfHeight), collQueryParam);

	for (size_t i = 0; i < hitResults.Num(); i++)
	{
		if (hitResults[i].bBlockingHit && IsValid(hitResults[i].GetComponent()) 
			&& !ObjHitted.Contains(hitResults[i].GetComponent()) 
			&& hitResults[i].GetComponent()->GetClass() == UStaticMeshComponent::StaticClass())
				ObjHitted.AddUnique(hitResults[i].GetComponent());		
	}

	for (size_t i = 0; i < ObjHitted.Num(); i++)
	{
		if (!ObjHittedTemp.Contains(ObjHitted[i]))
		{

			TArray<UMaterialInterface *> baseMat;
			TArray<UMaterialInstanceDynamic *> newMID;

			baseMat.Empty();
			newMID.Empty();

			for (int m = 0; m < ObjHitted[i]->GetNumMaterials(); m++) {
				baseMat.Add(ObjHitted[i]->GetMaterial(m));
				newMID.Add(UMaterialInstanceDynamic::Create(fadeMat, ObjHitted[i]));

				ObjHitted[i]->SetMaterial(m, newMID[m]);
			}

			FObjectStruct newObject;
			newObject.NewElement(ObjHitted[i], baseMat, newMID, 1.0, true);

			ObjHittedTemp.AddUnique(ObjHitted[i]);
			ObjToFadeArr.Add(newObject);
			ObjHitted[i]->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		}
	}

	for (size_t i = 0; i < ObjHittedTemp.Num(); i++)
	{
		if (!ObjHitted.Contains(ObjHittedTemp[i]))
		{
			ObjToFadeArr[i].SetHide(false);
		}
	}

	ObjHitted.Empty();
}

void UFadeObjects::FadeObjects()
{
	if (ObjToFadeArr.Num() > 0) {
		for (size_t i = 0; i < ObjToFadeArr.Num(); i++)
		{
			for (size_t m = 0; m < ObjToFadeArr[i].dynamicMat.Num(); m++)
			{
				float targetF;

				if (ObjToFadeArr[i].bToHide)
				{
					targetF = targetFade;
				}
				else
				{
					targetF = 1.0;
				}

				const float currFade = ObjToFadeArr[i].fadeCurr;

				const float newFade = FMath::FInterpConstantTo(currFade, targetF,
					GetWorld()->DeltaTimeSeconds, rateFade);

				tempFade = newFade;
				ObjToFadeArr[i].dynamicMat[m]->SetScalarParameterValue("Fade", newFade);
				ObjToFadeArr[i].fadeCurr = newFade;

			}
			if (tempFade == 1.0)
			{
				for (size_t m = 0; m < ObjToFadeArr[i].baseMatInterface.Num(); m++)
				{
					ObjToFadeArr[i].pComp->SetMaterial(m, ObjToFadeArr[i].baseMatInterface[m]);
				}

				ObjToFadeArr[i].pComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
				ObjToFadeArr.RemoveAt(i);
				ObjHittedTemp.RemoveAt(i);
			}
		}
	}
}

