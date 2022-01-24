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

	// ...
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
			UE_LOG(LogInteractionSystem, Log, TEXT("Interaction Sensor Component succesfully initalized on %s"),*GetOwner()->GetName())
		}
	}
}

void UInteractionSensorComponent::InteractionCheckLoop()
{
	AActor* ActorInView;

	//Perform hit scan to get hit actor
	if (GetHitActorInView(ActorInView))
	{

		//check if has interactable actor
		if (GetInteractableComponent(ActorInView, InteractableObjectComponentInView))
		{
			bInteractableObjectInView = true;
		}
		else
		{
			bInteractableObjectInView = false;
		}
	}
}

bool UInteractionSensorComponent::GetHitActorInView(AActor*& HitActor)
{

	FVector StartLocation;
	FRotator ViewRotation;
	FVector EndLocation;
	FHitResult Hit;
	bool TraceResult;
	FCollisionQueryParams TraceParams;

	TraceParams.bTraceComplex = true;

	OwningController->GetPlayerViewPoint(StartLocation, ViewRotation);
	EndLocation = StartLocation + (ViewRotation.Vector() * InteractionDistance);

	TraceResult = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, TraceParams);

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1.f);
	}

	if (TraceResult)
	{
		HitActor = Hit.GetActor();
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




