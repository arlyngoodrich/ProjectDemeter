// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSystem/StaminaComponent_C.h"
#include "Core/Logs_C.h"

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
		UE_LOG(LogStamina, Log, TEXT("%s stamina is now depleted"), *GetOwner()->GetName())
	}
	else
	{
		BP_OnStaminaRegenerated();
		UE_LOG(LogStamina, Log, TEXT("%s stamina is now regenerated"), *GetOwner()->GetName())
	}

}

void UStaminaComponent_C::OnSprintStart()
{

	
	StartStaminaDrain();
	StopStaminaRegeneration();
	bIsWalking = false;
	
}

void UStaminaComponent_C::OnRunStart()
{
	
	//Clear Stamina Drain
	StopStaminaRegeneration(); 
	StopStaminaDrain();
	bIsWalking = false;
}

void UStaminaComponent_C::OnWalkStart()
{

	StartStaminaRegeneration();
	StopStaminaDrain();	
	bIsWalking = true;
}

void UStaminaComponent_C::OnNoneStart()
{
}

void UStaminaComponent_C::RegenerateStamina()
{
	UE_LOG(LogStamina, Log, TEXT("%s regenerating %f stamina"), *GetOwner()->GetName(),StaminaRegenerationAmount)
	UpdateStamina(StaminaRegenerationAmount);

}

void UStaminaComponent_C::DrainSprintStamina()
{
	UE_LOG(LogStamina, Log, TEXT("%s draining %f stamina by sprinting"), *GetOwner()->GetName(), SprintStaminaDrain)
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
		UE_LOG(LogStamina, Log, TEXT("%s current stamina: %f"), *GetOwner()->GetName(), CurrentStamina);

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
	if (CurrentStamina == MaxStamina) { return; }
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




