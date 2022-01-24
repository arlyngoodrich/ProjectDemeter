// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem/InteractionSensorComponent.h"
#include "InteractionSystem/InteractableObjectComponent.h"
#include "Core/Logs_C.h"

//UE4 Includes 
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

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
	
	Initalize();
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

void UInteractionSensorComponent::Initalize()
{

	ACharacter* OwningCharacterCheck;
	APlayerController* OwningControllerCheck;

	OwningCharacterCheck = Cast<ACharacter>(GetOwner());

	if(!OwningCharacterCheck)
	{
		UE_LOG(LogInteractionSystem, Error, 
				TEXT("Interaction Sensor component does not have a character as owner.  Owner is %s"),
				*GetOwner()->GetName()) 
	}
	else
	{
		OwningControllerCheck = Cast<APlayerController>(OwningCharacterCheck->GetController());

		if (!OwningControllerCheck)
		{
			UE_LOG(LogInteractionSystem, Error,TEXT("Interaction Sensor Component does not have a player controller with it's owner."))
		}
		else
		{
			OwningController = OwningControllerCheck;
			UE_LOG(LogInteractionSystem, Log, TEXT("Interaction Sensor Component succesfully initalized on %s"), *GetOwner()->GetName())
				
			ToggleInteraction(true);
		}
	}
}

void UInteractionSensorComponent::Interact()
{
	if (bInteractableObjectInView)
	{
		//Interact
	}
	else
	{
		UE_LOG(LogInteractionSystem, Log, TEXT("No interactable in view for %s.  Disregarding request."), *GetOwner()->GetName())
	}
}

void UInteractionSensorComponent::ToggleInteraction(bool bShouldCheckForInteraction)
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
		//Otherwise don't check for interactable componet if it's the same
		if (HitActorInView != ActorInView)
		{

			//Tell old interactalbe no longer in view

			//Tell new interactable now in view


			ActorInView = HitActorInView;

			//check if has interactable actor
			if (GetInteractableComponent(HitActorInView, InteractableObjectComponentInView))
			{
				bInteractableObjectInView = true;
			}
			else
			{
				bInteractableObjectInView = false;
			}
		}
	}
}

bool UInteractionSensorComponent::GetHitActorInView(AActor*& HitActor)
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

bool UInteractionSensorComponent::GetInteractableComponent(AActor* HitActor, UInteractableObjectComponent*& HitActorInterableObjectComponent)
{

	HitActorInterableObjectComponent = HitActor->FindComponentByClass<UInteractableObjectComponent>();

	if (HitActorInterableObjectComponent)
	{
		return true;
	}
	else
	{
		return false;
	}
}




