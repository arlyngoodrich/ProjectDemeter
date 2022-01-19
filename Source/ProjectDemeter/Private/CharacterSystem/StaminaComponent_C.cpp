// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSystem/StaminaComponent_C.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UStaminaComponent_C::UStaminaComponent_C()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


	MaxStamina = 100;
	SprintStaminaDrain = 2;
	StaminaRegenerationAmount = 4;
	StaminaDepletionLevel = 80;

	// ...
}

void UStaminaComponent_C::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStaminaComponent_C, CurrentStamina);

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

void UStaminaComponent_C::OnRep_StaminaChange()
{
	BP_OnStaminaUpdate(CurrentStamina);
}

void UStaminaComponent_C::OnSprintStart()
{
	UE_LOG(LogTemp, Log, TEXT("On Sprint Start"));
	
	StartStaminaDrain();
	StopStaminaRegeneration();
	
}

void UStaminaComponent_C::OnRunStart()
{
	UE_LOG(LogTemp, Log, TEXT("On Run Start"));
	
	//Clear Stamina Drain
	StopStaminaRegeneration(); 
	StopStaminaDrain();
}

void UStaminaComponent_C::OnWalkStart()
{

	UE_LOG(LogTemp, Log, TEXT("On Walk Start"))
	StartStaminaRegeneration();
	StopStaminaDrain();
	
}

void UStaminaComponent_C::OnNoneStart()
{
	UE_LOG(LogTemp, Log, TEXT("On None Start"))
}

void UStaminaComponent_C::RegenerateStamina()
{
	UE_LOG(LogTemp, Log, TEXT("RegenerateStamina"))
	UpdateStamina(StaminaRegenerationAmount);

	if (bIsStaminaDepleted == true && CurrentStamina >= StaminaDepletionLevel)
	{
		BP_OnStaminaRegenerated();
		bIsStaminaDepleted = false;
	}
	
	//Stop Stamina Regen once it reaches max level
	if (CurrentStamina == MaxStamina)
	{
		StopStaminaRegeneration();
	}
}

void UStaminaComponent_C::DrainSprintStamina()
{
	UE_LOG(LogTemp, Log, TEXT("Drain Stamina"))
	UpdateStamina(-SprintStaminaDrain);

	if (bIsStaminaDepleted == false && CurrentStamina <= StaminaDepletionLevel)
	{
		BP_OnStaminaDepleted();
		bIsStaminaDepleted = true;
	}

}

void UStaminaComponent_C::UpdateStamina(float StaminaDelta)
{	
	float OldStamina = CurrentStamina;
	CurrentStamina = FMath::Clamp(OldStamina + StaminaDelta, 0.f, MaxStamina);

	if (OldStamina != CurrentStamina)
	{
		BP_OnStaminaUpdate(CurrentStamina);
		UE_LOG(LogTemp, Log, TEXT("Current Stamina: %f"), CurrentStamina);
	}

}

void UStaminaComponent_C::StartStaminaRegeneration()
{
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenerationTimer, this, &UStaminaComponent_C::RegenerateStamina, 1.f, true);
}

void UStaminaComponent_C::StopStaminaRegeneration()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenerationTimer);
}

void UStaminaComponent_C::StartStaminaDrain()
{
	GetWorld()->GetTimerManager().SetTimer(SprintStaminaDrainTimer, this, &UStaminaComponent_C::DrainSprintStamina, 1.f, true);
}

void UStaminaComponent_C::StopStaminaDrain()
{
	GetWorld()->GetTimerManager().ClearTimer(SprintStaminaDrainTimer);
}




