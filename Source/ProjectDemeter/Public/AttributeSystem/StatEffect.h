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

	virtual void InitalizeEffect(AActor* TargetActor, AController* InstigatingController);

	bool bReadyToTriggerEffect;

	virtual bool TriggerEffect();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Stat Effect")
	TSubclassOf<UBaseStatComponent> TargetComponentClass;
	
	//Must be negative to decrease and positve to increase
	UPROPERTY(EditDefaultsOnly, Category = "Stat Effect")
	float StatEffectAmount;

	bool GetTargetComponent(AActor* TargetActor, UBaseStatComponent*& OutTargetComponent);

	UBaseStatComponent* TargetComponent;


};
