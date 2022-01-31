// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatEffect.generated.h"

class UBaseStatComponent;
class AActor;

/**
 * 
 */
UCLASS(blueprintable)
class PROJECTDEMETER_API UStatEffect : public UObject
{
	GENERATED_BODY()

public:
	UStatEffect();

	virtual void InitializeEffect(AActor* TargetActor);

	bool bReadyToTriggerEffect;

	//Checks to make sure target component is valid before being overriden by child classes for implementation
	virtual bool TriggerEffect();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Stat Effect")
	TSubclassOf<UBaseStatComponent> TargetComponentClass;

	UPROPERTY()
	UBaseStatComponent* TargetComponent;
	
	bool GetTargetComponent(const AActor* TargetActor, UBaseStatComponent*& OutTargetComponent) const;
	
};
