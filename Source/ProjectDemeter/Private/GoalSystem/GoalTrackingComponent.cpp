// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalSystem/GoalTrackingComponent.h"
#include "GoalSystem/GoalObjectBase.h"
#include "Core/Logs_C.h"

//UE4 Includes
#include "GameFramework/PlayerController.h"

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

	// ...
	
}

void UGoalTrackingComponent::Initialize()
{
	OwningPlayer = Cast<APlayerController>(GetOwner());
	if (OwningPlayer == nullptr)
	{
		UE_LOG(LogGoalSystem,Error,TEXT("%s must have player controller as owner"),*GetClass()->GetName());
	}
}

void UGoalTrackingComponent::Internal_AddGoal(TSubclassOf<UGoalObjectBase> GoalToAdd)
{
	//Initialize Goal 
	UGoalObjectBase* NewGoalObject = NewObject<UGoalObjectBase>(this,GoalToAdd);
	NewGoalObject->Initialize(OwningPlayer);
	
	//Add to tracking array
	ActiveGoals.Add(NewGoalObject);
	UE_LOG(LogGoalSystem,Log,TEXT("%s was added to %s goal tracking"),*NewGoalObject->GetName(),*OwningPlayer->GetName());
	
}

void UGoalTrackingComponent::Internal_RemoveGoal(UGoalObjectBase* GoalToRemove)
{

	if(GoalToRemove == nullptr)
	{
		UE_LOG(LogGoalSystem,Error,TEXT("Attempting to remove null goal object from %s"),*OwningPlayer->GetName())
		return;
	}
	//verify goal in array
	if(ActiveGoals.Find(GoalToRemove))
	{
		//remove from array
		ActiveGoals.Remove(GoalToRemove);
		UE_LOG(LogGoalSystem,Log,TEXT("%s was removed from %s goals"),*GoalToRemove->GetName(),*OwningPlayer->GetName())
		
		//mark for Garbage Collection
		GoalToRemove->MarkPendingKill();
	}
	else
	{
		UE_LOG(LogGoalSystem,Warning,TEXT("Goal object %s not found in %s Active Goal"),*GoalToRemove->GetName(),*OwningPlayer->GetName());
		return;
	}
}

