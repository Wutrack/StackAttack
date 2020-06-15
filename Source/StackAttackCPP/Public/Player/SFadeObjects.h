// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFadeObjects.generated.h"

USTRUCT()
struct FObjectStruct {
	GENERATED_USTRUCT_BODY()

	UPrimitiveComponent							*pComp;

	TArray<class UMaterialInterface *>			baseMatInterface;

	TArray<class UMaterialInstanceDynamic *>	dynamicMat;

	float										fadeCurr;
	
	bool										bToHide;


	void NewElement(UPrimitiveComponent *newComponent, TArray<UMaterialInterface *> newBaseMat,
		TArray<UMaterialInstanceDynamic *> newMID, float currentFade, bool bHide)
	{
		pComp					= newComponent;
		baseMatInterface		= newBaseMat;
		dynamicMat				= newMID;
		fadeCurr				= currentFade;
		bToHide					= bHide;
	}

	void SetHide(bool hide)
	{
		bToHide					= hide;
	}

	void SetFadeAndHide(float newFade, bool newHide)
	{
		fadeCurr				= newFade;
		bToHide					= newHide;
	}

	//For Destroy
	void Destroy()
	{
		pComp					= nullptr;
	}

	//Constructor
	FObjectStruct()
	{
		pComp					= nullptr;
		fadeCurr				= 0;
		bToHide					= true;
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STACKATTACKCPP_API USFadeObjects : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USFadeObjects();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	void				CheckObjects();
	void				FadeObjects();

	TArray<FObjectStruct>					ObjToFadeArr;
	TArray<UPrimitiveComponent *>			ObjHitted;
	TArray<UPrimitiveComponent *>			ObjHittedTemp;

	FTimerHandle			CheckObjHandle;
	FTimerHandle			FadeObjHandle;
	float					CheckInterval			= .1f, 
							FadeInterval			= .1f;

	float					TargetFade				= .2f,
							RateFade				= 100.f,
							TempFade;

	float					CapsuleHalfHeight		= 50.f, 
							CapsuleRadius			= 30.f;

	class UMaterial			*FadeMaterial				= nullptr;
	AActor					*Player					= nullptr;

	TArray<TEnumAsByte<ECollisionChannel>>			ObjectTypes;




};
