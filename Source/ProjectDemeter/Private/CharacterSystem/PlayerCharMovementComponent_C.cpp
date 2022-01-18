// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSystem/PlayerCharMovementComponent_C.h"
#include "CharacterSystem/PlayerCharacter_C.h"

void UPlayerCharMovementComponent_C::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<APlayerCharacter_C>(PawnOwner);

	DefaultMaxSpeed = OwningCharacter->GetDefaultWalkSpeed();

}

float UPlayerCharMovementComponent_C::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	if (OwningCharacter)
	{
		if (OwningCharacter->GetWantsToSprint() && OwningCharacter->bIsCrouched == false)
		{
			MaxSpeed = DefaultMaxSpeed * OwningCharacter->GetSprintSpeedModifier();
		}
		else
		{
			MaxSpeed = DefaultMaxSpeed;
		}
	}

	return MaxSpeed;
}