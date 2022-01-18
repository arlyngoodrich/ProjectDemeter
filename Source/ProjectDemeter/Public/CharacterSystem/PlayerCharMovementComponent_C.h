// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharMovementComponent_C.generated.h"

/**
 * 
 */

class APlayerCharacter_C;

UCLASS()
class PROJECTDEMETER_API UPlayerCharMovementComponent_C : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual float GetMaxSpeed() const override;

protected:

	APlayerCharacter_C* OwningCharacter;

	float DefaultMaxSpeed;
};
