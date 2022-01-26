// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/BaseStatComponent.h"

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

void UBaseStatComponent::OnRep_CurrentValueChange()
{
	OnCurrentValueChange.Broadcast();
}


void UBaseStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseStatComponent, CurrentValue);

}