// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSystem/StaminaComponent_C.h"

// Sets default values for this component's properties
UStaminaComponent_C::UStaminaComponent_C()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


	MaxStamina = 100;
	SprintStaminaDrain = 2;
	StaminaRegenerationAmount = 4;

	// ...
}


// Called when the game starts
void UStaminaComponent_C::BeginPlay()
{
	Super::BeginPlay();

	CurrentStamina = MaxStamina;

	// ...
	
}

void UStaminaComponent_C::OnGaitUpdate(EGait NewGait)
{

	//UE_LOG(LogTemp, Log, TEXT("Gait Enum is %s"), *UEnum::GetValueAsString(NewGait));

	switch (NewGait)
	{
	case EGait::EG_None:
		OnNoneStart();
		break;
	case EGait::EG_Walk:
		OnWalkStart();
		break;
	case EGait::EG_Run:
		OnRunStart();
		break;
	case EGait::EG_Sprint:
		OnSprintStart();
		break;
	default:
		break;
	}

}

void UStaminaComponent_C::OnSprintStart()
{
	UE_LOG(LogTemp, Log, TEXT("On Sprint Start"));
	
	//Start stamian drain timer
	GetWorld()->GetTimerManager().SetTimer(SprintStaminaDrainTimer, this, &UStaminaComponent_C::DrainSprintStamina, 1.f, true);
	
	//Clear Stamina Regen
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenerationTimer);
	
}

void UStaminaComponent_C::OnRunStart()
{
	UE_LOG(LogTemp, Log, TEXT("On Run Start"));
	
	//Clear Stamina Drain
	GetWorld()->GetTimerManager().ClearTimer(SprintStaminaDrainTimer);

}

void UStaminaComponent_C::OnWalkStart()
{

	//Start Stamina Regen
	UE_LOG(LogTemp, Log, TEXT("On Walk Start"))
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenerationTimer, this, &UStaminaComponent_C::RegenerateStamina, 1.f, true);
	
	//Clear Stamina Drain
	GetWorld()->GetTimerManager().ClearTimer(SprintStaminaDrainTimer);
	
}

void UStaminaComponent_C::OnNoneStart()
{
	UE_LOG(LogTemp, Log, TEXT("On None Start"))
}

void UStaminaComponent_C::RegenerateStamina()
{
	UE_LOG(LogTemp, Log, TEXT("RegenerateStamina"))
	UpdateStamina(StaminaRegenerationAmount);
	
	//Stop Stamina Regen once it reaches max level
	if (CurrentStamina == MaxStamina)
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenerationTimer);
	}
}

void UStaminaComponent_C::DrainSprintStamina()
{
	UE_LOG(LogTemp, Log, TEXT("Drain Stamina"))
	UpdateStamina(-SprintStaminaDrain);
}

void UStaminaComponent_C::UpdateStamina(float StaminaDelta)
{	
	float OldStamina = CurrentStamina;
	CurrentStamina = FMath::Clamp(OldStamina + StaminaDelta, 0.f, MaxStamina);
	UE_LOG(LogTemp, Log, TEXT("Current Stamina: %f"), CurrentStamina);
}




