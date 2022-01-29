#include "AttributeSystem/StaminaStat.h"
#include "Core/Logs_C.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"

UStaminaStat::UStaminaStat()
{
    MaxValue = 100;
    bShouldRegenerate = true;
    SprintStaminaDrain = 10.f;
    JumptStaminaCost = 20.f;
    RollStmainaCost = 20.f;
}

void UStaminaStat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UStaminaStat, bIsStaminaDepleted);

}

float UStaminaStat::GetCurrentStamina(){ return CurrentValue;}

float UStaminaStat::GetMaxStamina() { return MaxValue;}

void UStaminaStat::BeginPlay()
{
	Super::BeginPlay();
	// ...
}

void UStaminaStat::OnJump()
{
    ChangeCurrentValue(-JumptStaminaCost);
}


void UStaminaStat::OnRoll()
{
    ChangeCurrentValue(-RollStmainaCost);
}


 void UStaminaStat::OnCustomAction(float StaminaCost)
 {
     ChangeCurrentValue(-StaminaCost);
 }


void UStaminaStat::OnGaitUpdate(EGait_New NewGait)
{

	switch (NewGait)
	{
	case EGait_New::EG_None:
		break;
	case EGait_New::EG_Walk:
		OnWalkStart();
		break;
	case EGait_New::EG_Run:
		OnRunStart();
		break;
	case EGait_New::EG_Sprint:
		OnSprintStart();
		break;
	default:
		break;
	}

}


void UStaminaStat::OnSprintStart()
{
    UE_LOG(LogAttributeSystem,Log,TEXT("%s started to sprint"), *GetOwner()->GetName())
    ToggleRegeneration(false);
    ToggleSprintStaminaDrain(true);
    bIsWalking = false;
}

void UStaminaStat::OnRunStart()
{
    UE_LOG(LogAttributeSystem, Log, TEXT("%s started to run"), *GetOwner()->GetName())
    ToggleRegeneration(false);
    ToggleSprintStaminaDrain(false);
    bIsWalking = false;
}

void UStaminaStat::OnWalkStart()
{
    UE_LOG(LogAttributeSystem, Log, TEXT("%s started to walk"), *GetOwner()->GetName())
    ToggleRegeneration(true);
    ToggleSprintStaminaDrain(false);
    bIsWalking = true;
}


void UStaminaStat::OnCurrentValueChange()
{

    Super::OnCurrentValueChange();

    //check if stamina is newly depleted
    if(CurrentValue == 0 && bIsStaminaDepleted == false)
    {
        bIsStaminaDepleted = true;
        OnRep_StaminaDepletedUpdated();
    }
    else if(CurrentValue > 0 && bIsStaminaDepleted == true)
    {
        bIsStaminaDepleted = false;
        OnRep_StaminaDepletedUpdated();
    }


    //Make sure stamina starts regenerating if the charcter is only walking.  
    if(bIsWalking)
    {
        ToggleRegeneration(true);
    }
}


void UStaminaStat::OnRep_StaminaDepletedUpdated()
{
    if(bIsStaminaDepleted)
    {
        //If Stamina is depleted
        OnStaminaDepletedChangeDelegate.Broadcast(true);
        UE_LOG(LogAttributeSystem,Log,TEXT("Stamina is depleted for %s"),*GetOwner()->GetName())
    }
    else
    {   
        //If Stamina is no longer depleted
        OnStaminaDepletedChangeDelegate.Broadcast(false);
        UE_LOG(LogAttributeSystem,Log,TEXT("Stamina is no longer depleted for %s"),*GetOwner()->GetName())
    }
}


void UStaminaStat::ToggleSprintStaminaDrain(bool bDrain)
{
   

    if(bDrain)
    {
        UE_LOG(LogAttributeSystem, Log, TEXT("%s called start sprint stamina drian"), *GetOwner()->GetName())

        //Check to see if already draining
        if(GetWorld()->GetTimerManager().IsTimerActive(SprintStaminaDrainTimer) == false)
        {   
            //if not, then drain
            GetWorld()->GetTimerManager().SetTimer(SprintStaminaDrainTimer, this, &UStaminaStat::DrainSprintStamina, 1.f, true);
        }          
    }
    else
    {
        //Check to see if alrady draining
        if(GetWorld()->GetTimerManager().IsTimerActive(SprintStaminaDrainTimer) == true)
        {
            //if not, then drain
            GetWorld()->GetTimerManager().ClearTimer(SprintStaminaDrainTimer);
        }   
    }
}


 void UStaminaStat::DrainSprintStamina()
 {
     ChangeCurrentValue(-SprintStaminaDrain);
 }