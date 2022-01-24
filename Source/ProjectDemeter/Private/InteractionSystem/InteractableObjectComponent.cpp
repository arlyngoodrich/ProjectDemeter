// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem/InteractableObjectComponent.h"
#include "Core/Logs_C.h"


// Sets default values for this component's properties
UInteractableObjectComponent::UInteractableObjectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UInteractableObjectComponent::Interact()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		UE_LOG(LogInteractionSystem,Log,TEXT("Interaction called on %s"),*GetOwner()->GetName())
	}
	else
	{
		UE_LOG(LogInteractionSystem,Warning,TEXT("Attempting to call interact with non-authority for %s"),*GetOwner()->GetName())
	}

}

void UInteractableObjectComponent::ToggleFocus(bool bIsInFocus)
{
}

// Called when the game starts
void UInteractableObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


