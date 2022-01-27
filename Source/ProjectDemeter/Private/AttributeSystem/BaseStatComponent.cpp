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
	Initialize();

	// ...
	
}

void UBaseStatComponent::Initialize()
{
	CurrentValue = MaxValue;
}

void UBaseStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseStatComponent, CurrentValue);

}

void UBaseStatComponent::EffectStat(float EffectAmount)
{
	ChangeCurrentValue(EffectAmount);
}

void UBaseStatComponent::OnRep_CurrentValueChange()
{
	OnCurrentValueChange.Broadcast();
	BP_OnCurrentValueChange();
}

void UBaseStatComponent::MaxValueReached()
{
	ToggleRegeneration(false);
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

	//Check to see if regeneration should be started and if its OK to regenerate
	if (bShouldStart && bShouldRegenerate)
	{
		//Check to see if it's already running
		if (GetWorld()->GetTimerManager().IsTimerActive(RegenerationTimer) == false)
		{
			//If not, then start it 
			GetWorld()->GetTimerManager().SetTimer(RegenerationTimer, this, &UBaseStatComponent::RegenerateValue, 1.f, true);
			UE_LOG(LogAttributeSystem,Log,TEXT("Regneration started on %s component for %s"),*GetClass()->GetNamer(),*GetOwner()->GetNamer());
		}

		return;
	}
	//if not, then stop regeneration if it's currently running
	else
	{	//Check to see if currently regenerating
		if (GetWorld()->GetTimerManager().TimerExists(RegenerationTimer) == true)
		{

			//If currentliy regenerating, then clear it
			GetWorld()->GetTimerManager().ClearTimer(RegenerationTimer);
			UE_LOG(LogAttributeSystem,Log,TEXT("Regneration stopped on %s component for %s"),*GetClass()->GetNamer(),*GetOwner()->GetNamer());
		}

		return;
	}
}

void UBaseStatComponent::RegenerateValue()
{
	ChangeCurrentValue(BaseRegenerationAmount);
}
