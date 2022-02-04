// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalSystem/GoalTrackingComponent.h"
#include "GoalSystem/GoalObjectBase.h"
#include "Core/Logs_C.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UGoalTrackingComponent::UGoalTrackingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UGoalTrackingComponent::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void UGoalTrackingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Different than default notify, makes replication updates whenever goal data is updated in array
	DOREPLIFETIME_CONDITION_NOTIFY(UGoalTrackingComponent, ActiveGoalData, COND_None, REPNOTIFY_Always);
	//DOREPLIFETIME(UGoalTrackingComponent,ActiveGoalData);
	
}

void UGoalTrackingComponent::Initialize()
{
	OwningActor = GetOwner();
}

void UGoalTrackingComponent::BP_AddGoal(TSubclassOf<UGoalObjectBase> GoalToAdd)
{
	Internal_AddGoal(GoalToAdd);
}

void UGoalTrackingComponent::OnRep_GoalDataUpdate() const
{
	OnGoalDataUpdateDelegate.Broadcast();
}

void UGoalTrackingComponent::Internal_AddGoal(const TSubclassOf<UGoalObjectBase> GoalToAdd)
{
	if(!IsValid(GoalToAdd))
	{
		UE_LOG(LogGoalSystem,Error,TEXT("%s attempted to add invalid goal"),*OwningActor->GetName())
		return;
	}

	if (OwningActor == nullptr)
	{
		UE_LOG(LogGoalSystem,Error,TEXT("%s attempted to add goal without player controller reference"),*GetClass()->GetName())
		return;
	}
	
	if(OwningActor->GetLocalRole()!=ROLE_Authority)
	{
		UE_LOG(LogGoalSystem,Error,TEXT("%s attempted to add goal as non-authority"),*OwningActor->GetName())
		return;
	}
	
	//Initialize Goal 
	UGoalObjectBase* NewGoalObject = NewObject<UGoalObjectBase>(this,GoalToAdd);
	NewGoalObject->Initialize(OwningActor,this,false);
	
	//Add to goal object to tracking array
	ActiveGoals.Add(NewGoalObject);

	//Add goal data to goal data array for replication
	ActiveGoalData.Add(NewGoalObject->GoalData);

	OnRep_GoalDataUpdate();

	UE_LOG(LogGoalSystem, Log, TEXT("%s was added to %s goal tracking.  GUID = %s"), *NewGoalObject->GetName(),
	       *OwningActor->GetName(), *NewGoalObject->GoalData.GoalGUID.ToString(EGuidFormats::Digits));
}

void UGoalTrackingComponent::Internal_RemoveGoal(UGoalObjectBase* GoalToRemove)
{

	if(GoalToRemove == nullptr)
	{
		UE_LOG(LogGoalSystem,Error,TEXT("Attempting to remove null goal object from %s"),*OwningActor->GetName())
		return;
	}
	//verify goal in array
	if(ActiveGoals.Find(GoalToRemove))
	{
		//remove object from active object goal array
		ActiveGoals.Remove(GoalToRemove);

		//remove data from active goal data array 
		int32 GoalDataIndex;
		const FGuid GUID = GoalToRemove->GoalData.GoalGUID;
		FindGoalDataByGUID(GUID,GoalDataIndex);
		ActiveGoalData.RemoveAt(GoalDataIndex);
		
		OnRep_GoalDataUpdate();

		UE_LOG(LogGoalSystem, Log, TEXT("%s with GUID:%s was removed from %s goals"), *GoalToRemove->GetName(),
		       *GUID.ToString(), *OwningActor->GetName())

		//mark for Garbage Collection
		GoalToRemove->MarkPendingKill();
	}
	else
	{
		UE_LOG(LogGoalSystem, Warning, TEXT("Goal object %s not found in %s Active Goal"), *GoalToRemove->GetName(),
		       *OwningActor->GetName());
		return;
	}
}

bool UGoalTrackingComponent::FindGoalDataByGUID(const FGuid GUID, int32& OutGoalDataIndex) const
{
	for (int i = 0; i < ActiveGoalData.Num(); ++i)
	{
		if(ActiveGoalData[i].GoalGUID == GUID)
		{
			OutGoalDataIndex = i;
			return true;
		}
	}
	
	return false;
}

void UGoalTrackingComponent::OnGoalDataUpdate(FGoalData GoalData)
{
}

