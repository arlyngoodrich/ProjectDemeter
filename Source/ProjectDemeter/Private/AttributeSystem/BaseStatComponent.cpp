// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/BaseStatComponent.h"
#include "Core/Logs_C.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UBaseStatComponent::UBaseStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBaseStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UBaseStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseStatComponent, CurrentValue);

}

void UBaseStatComponent::OnRep_CurrentValueChange()
{
	OnCurrentValueChange.Broadcast();
	BP_OnCurrentValueChange();
}

void UBaseStatComponent::MaxValueReached()
{
}

void UBaseStatComponent::ChangeCurrentValue(float DeltaAmount)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("Attempting to change stat value in %s on %s with non-Authority"), *GetClass()->GetName(), *GetOwner()->GetName())
			return;
	}

	float oldCurrentValue = CurrentValue;
	float newCurrentValue = FMath::Clamp(CurrentValue + DeltaAmount, 0.f, MaxValue);
	
	//Make sure value changed
	if (oldCurrentValue != newCurrentValue)
	{
		CurrentValue = newCurrentValue;
		OnRep_CurrentValueChange();

		if (CurrentValue == MaxValue)
		{
			MaxValueReached();
		}
	}

}

void UBaseStatComponent::ToggleRegeneration(bool bShouldStart)
{

	if (bShouldStart)
	{
		//Check to see if it's already running
		if (GetWorld()->GetTimerManager().IsTimerActive(RegenerationTimer) == false)
		{
			//If not, then start it 
			GetWorld()->GetTimerManager().SetTimer(RegenerationTimer, this, &UBaseStatComponent::RegenerateValue, 1.f, true);
		}

		return;
	}
	else
	{	//Check to see if it's 
		if (GetWorld()->GetTimerManager().TimerExists(RegenerationTimer) == true)
		{

			//If it does, then clear it
			GetWorld()->GetTimerManager().ClearTimer(RegenerationTimer);
		}

		return;
	}
}

void UBaseStatComponent::RegenerateValue()
{
	ChangeCurrentValue(BaseRegenerationAmount);
}
