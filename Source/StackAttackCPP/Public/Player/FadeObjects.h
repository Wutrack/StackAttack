// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FadeObjects.generated.h"

USTRUCT()
struct FObjectStruct {
	GENERATED_USTRUCT_BODY()

	UPrimitiveComponent *pComp;

	TArray<class UMaterialInterface *> baseMatInterface;

	TArray<class UMaterialInstanceDynamic *> dynamicMat;

	float fadeCurr;
	
	bool bToHide;


	void NewElement(UPrimitiveComponent *newComponent, TArray<UMaterialInterface *> newBaseMat,
		TArray<UMaterialInstanceDynamic *> newMID, float currentFade, bool bHide)
	{
		pComp = newComponent;
		baseMatInterface = newBaseMat;
		dynamicMat = newMID;
		fadeCurr = currentFade;
		bToHide = bHide;
	}

	void SetHide(bool hide)
	{
		bToHide = hide;
	}

	void SetFadeAndHide(float newFade, bool newHide)
	{
		fadeCurr = newFade;
		bToHide = newHide;
	}

	//For Destroy
	void Destroy()
	{
		pComp = nullptr;
	}

	//Constructor
	FObjectStruct()
	{
		pComp = nullptr;
		fadeCurr = 0;
		bToHide = true;
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STACKATTACKCPP_API UFadeObjects : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFadeObjects();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	void CheckObjects();
	void FadeObjects();

	TArray<FObjectStruct> ObjToFadeArr;
	TArray<UPrimitiveComponent *> ObjHitted;
	TArray<UPrimitiveComponent *> ObjHittedTemp;

	FTimerHandle checkObjHandle;
	FTimerHandle fadeObjHandle;
	float checkInterval, fadeInterval;

	float targetFade;
	float rateFade;
	float tempFade;

	class UMaterial *fadeMat;

	TArray<TEnumAsByte<ECollisionChannel>> objectTypes;

	AActor *player;

	float capsuleHalfHeight, capsuleRadius;


};
