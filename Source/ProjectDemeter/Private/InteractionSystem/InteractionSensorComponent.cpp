// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem/InteractionSensorComponent.h"
#include "InteractionSystem/InteractableObjectComponent.h"
#include "CharacterSystem/Character_C.h"
#include "Core/Logs_C.h"

//UE4 Includes 
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInteractionSensorComponent::UInteractionSensorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	InteractionDistance = 500.f;
	bDrawDebug = true;
	// ...
}


// Called when the game starts
void UInteractionSensorComponent::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

// Called every frame
void UInteractionSensorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bShouldCheckForInteractable)
	{
		InteractionCheckLoop();
	}

}

void UInteractionSensorComponent::Initialize()
{
	
	ACharacter_C* OwningCharacterCheck = Cast<ACharacter_C>(GetOwner());

	if(!OwningCharacterCheck)
	{
		UE_LOG(LogInteractionSystem, Error, 
				TEXT("Interaction Sensor component does not have a character as owner.  Owner is %s"),
				*GetOwner()->GetName()) 
	}
	else
	{
		APlayerController* OwningControllerCheck = Cast<APlayerController>(OwningCharacterCheck->GetController());

		if (!OwningControllerCheck)
		{
			//Not controlled by player character
			//Check to see if ever possessed by player
			UE_LOG(LogInteractionSystem, Log,
			       TEXT("Interaction systm on %s not possessed by player.  Will wait for player possession"),
			       *GetOwner()->GetName())
			OwningCharacterCheck->OnCharacterPossessedDelegate.AddDynamic(this,&UInteractionSensorComponent::OnNewPossession);
		}
		else
		{
			OwningController = OwningControllerCheck;
			

			if(OwningController->IsLocalPlayerController())
			{
				ToggleInteraction(true);
				UE_LOG(LogInteractionSystem, Log, TEXT("Interaction Sensor Component succesfully initalized on %s"), *GetOwner()->GetName())
			}		
		}
	}
}

void UInteractionSensorComponent::Interact()
{
	if (bInteractableObjectInView)
	{
		if (GetOwnerRole() == ROLE_Authority)
		{
			TriggerInteraction(InteractableObjectComponentInView);
		}
		else
		{
			Server_TriggerInteraction(InteractableObjectComponentInView);
		}
	}
	else
	{
		UE_LOG(LogInteractionSystem, Log, TEXT("No interactable in view for %s.  Disregarding request."), *GetOwner()->GetName())
	}
}

void UInteractionSensorComponent::TriggerInteraction(UInteractableObjectComponent* ComponentInView) const
{
	ComponentInView->Interact(GetOwner());
}

bool UInteractionSensorComponent::Server_TriggerInteraction_Validate(UInteractableObjectComponent* ComponentInView)
{
	return true;
}

void UInteractionSensorComponent::Server_TriggerInteraction_Implementation(UInteractableObjectComponent* ComponentInView)
{
	TriggerInteraction(ComponentInView);
}

void UInteractionSensorComponent::ToggleInteraction(const bool bShouldCheckForInteraction)
{
	bShouldCheckForInteractable = bShouldCheckForInteraction;

	if (bShouldCheckForInteraction)
	{
		UE_LOG(LogInteractionSystem,Log,TEXT("Interaction Started for %s"),*GetOwner()->GetName())
	}
	else
	{
		UE_LOG(LogInteractionSystem,Log,TEXT("Interaction Stopped for %s"),*GetOwner()->GetName())
	}
}

void UInteractionSensorComponent::InteractionCheckLoop()
{
	AActor* HitActorInView;

	//Perform hit scan to get hit actor
	if (GetHitActorInView(HitActorInView))
	{
		//If Actor in view is not the same as the new actor in view, set as new one.  
		//Otherwise don't check for interactable component if it's the same
		if (HitActorInView != ActorInView)
		{

			ActorInView = HitActorInView;

			//check if new actor has interactable actor
			UInteractableObjectComponent* NewInteractableObject;

			if (GetInteractableComponent(HitActorInView, NewInteractableObject))
			{
				//Tell old interactable no longer in focus
				if (InteractableObjectComponentInView)
				{
					InteractableObjectComponentInView->ToggleFocus(false);
				}
				
				
				InteractableObjectComponentInView = NewInteractableObject;
				InteractableObjectComponentInView->ToggleFocus(true);
				bInteractableObjectInView = true;
				return;
			}
		}
		else
		{
			//If the actor is the same, no need to make any changes
			return;
		}
	} 	//If there was previously an interactable in view but is no longer
	else if (InteractableObjectComponentInView)
	{
		InteractableObjectComponentInView->ToggleFocus(false);
		InteractableObjectComponentInView = nullptr;
		ActorInView = nullptr;
		bInteractableObjectInView = false;
	}

}

bool UInteractionSensorComponent::GetHitActorInView(AActor*& HitActor) const
{

	if (!OwningController) { return false; }


	FVector StartLocation;
	FRotator ViewRotation;
	FVector EndLocation;
	FHitResult Hit;
	bool TraceResult;
	FCollisionQueryParams TraceParams;

	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(GetOwner());

	OwningController->GetPlayerViewPoint(StartLocation, ViewRotation);
	EndLocation = StartLocation + (ViewRotation.Vector() * InteractionDistance);

	TraceResult = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, TraceParams);

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, .1f);
	}

	if (TraceResult)
	{
		HitActor = Hit.GetActor();

		if (bDrawDebug)
		{
			DrawDebugBox(GetWorld(), Hit.Location, FVector(10.f, 10.f, 10.f), FColor::Red, false, .1f);
		}

		return true;
	}
	else
	{
		HitActor = nullptr;
		return false;
	}
}

bool UInteractionSensorComponent::GetInteractableComponent(const AActor* HitActor,
                                                           UInteractableObjectComponent*&
                                                           HitActorInteractableObjectComponent)
{

	HitActorInteractableObjectComponent = HitActor->FindComponentByClass<UInteractableObjectComponent>();

	if (HitActorInteractableObjectComponent)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UInteractionSensorComponent::OnNewPossession(AController* NewController)
{
	UE_LOG(LogInteractionSystem,Log,TEXT("New possession called for %s"),*GetOwner()->GetName())
	Initialize();
	
}




