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
	StaminaDepletionLevel = 20;
	JumpStaminaDrain = 10;

	// ...
}

void UStaminaComponent_C::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStaminaComponent_C, CurrentStamina);
	DOREPLIFETIME(UStaminaComponent_C, bIsStaminaDepleted);

}


bool UStaminaComponent_C::GetIsStaminaDepleted()
{
	return bIsStaminaDepleted;
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


void UStaminaComponent_C::OnJump()
{
	UpdateStamina(-JumpStaminaDrain);

	if (bIsWalking)
	{
		StartStaminaRegeneration();
	}
}

void UStaminaComponent_C::OnRoll()
{
	UpdateStamina(-RollStaminaDrain);

	if (bIsWalking)
	{
		StartStaminaRegeneration();
	}
}

void UStaminaComponent_C::OnCustomAction(float StaminaCost)
{
	UpdateStamina(-StaminaCost);

	if (bIsWalking)
	{
		StartStaminaRegeneration();
	}
}

void UStaminaComponent_C::OnRep_StaminaChange()
{
	BP_OnStaminaUpdate(CurrentStamina);
}

void UStaminaComponent_C::OnRep_IsDepletedUpdated()
{
	if (bIsStaminaDepleted)
	{
		BP_OnStaminaDepleted();
	}
	else
	{
		BP_OnStaminaRegenerated();
	}

}

void UStaminaComponent_C::OnSprintStart()
{
	UE_LOG(LogTemp, Log, TEXT("On Sprint Start"));
	
	StartStaminaDrain();
	StopStaminaRegeneration();
	bIsWalking = false;
	
}

void UStaminaComponent_C::OnRunStart()
{
	UE_LOG(LogTemp, Log, TEXT("On Run Start"));
	
	//Clear Stamina Drain
	StopStaminaRegeneration(); 
	StopStaminaDrain();
	bIsWalking = false;
}

void UStaminaComponent_C::OnWalkStart()
{

	UE_LOG(LogTemp, Log, TEXT("On Walk Start"))
	StartStaminaRegeneration();
	StopStaminaDrain();	
	bIsWalking = true;
}

void UStaminaComponent_C::OnNoneStart()
{
	UE_LOG(LogTemp, Log, TEXT("On None Start"))
}

void UStaminaComponent_C::RegenerateStamina()
{
	UE_LOG(LogTemp, Log, TEXT("RegenerateStamina"))
	UpdateStamina(StaminaRegenerationAmount);

}

void UStaminaComponent_C::DrainSprintStamina()
{
	UE_LOG(LogTemp, Log, TEXT("Drain Sprint Stamina"))
	UpdateStamina(-SprintStaminaDrain);

}

void UStaminaComponent_C::UpdateStamina(float StaminaDelta)
{	
	float OldStamina = CurrentStamina;
	CurrentStamina = FMath::Clamp(OldStamina + StaminaDelta, 0.f, MaxStamina);

	//If stamina has changed
	if (OldStamina != CurrentStamina)
	{
		BP_OnStaminaUpdate(CurrentStamina);
		UE_LOG(LogTemp, Log, TEXT("Current Stamina: %f"), CurrentStamina);

		//Check to see if stamina should be depleted
		if (bIsStaminaDepleted == false && CurrentStamina <= StaminaDepletionLevel)
		{
			bIsStaminaDepleted = true;
			OnRep_IsDepletedUpdated();
		}

		//Check to see if stamina should be regenerated
		if (bIsStaminaDepleted == true && CurrentStamina >= StaminaDepletionLevel)
		{
			bIsStaminaDepleted = false;
			OnRep_IsDepletedUpdated();
		}

		//Stop Stamina Regen once it reaches max level
		if (CurrentStamina == MaxStamina)
		{
			StopStaminaRegeneration();
		}
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




