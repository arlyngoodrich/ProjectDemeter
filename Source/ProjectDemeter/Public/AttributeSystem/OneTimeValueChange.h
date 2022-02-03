// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/StatEffect.h"
#include "OneTimeValueChange.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDEMETER_API UOneTimeValueChange : public UStatEffect
{
	GENERATED_BODY()

protected:

	virtual bool TriggerEffect() override;
	
	//Must be negative to decrease and positive to increase
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat Effect")
	float StatEffectAmount;
	
	
};
