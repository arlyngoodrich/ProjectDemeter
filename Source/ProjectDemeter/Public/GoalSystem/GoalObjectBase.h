// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GoalSystem/GoalData.h"
#include "GoalObjectBase.generated.h"

class UGoalTrackingComponent;

/**
 * 
 */

UCLASS(blueprintable)
class PROJECTDEMETER_API UGoalObjectBase : public UObject
{
	GENERATED_BODY()

public:

	//Default Constructor
   UGoalObjectBase();

	//Goal data that will be shown on UI
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Goal System")
   FGoalData GoalData;

	//Called by spawning object
   virtual void Initialize(AActor* OwningActor,UGoalTrackingComponent* GoalTrackingComponent,bool bSetIsSubGoal);

protected:

	//Will typically be a player but may be an actor if used for testing
    UPROPERTY(BlueprintReadOnly, Category = "Goal System")
    AActor* OwningPlayer;

	//Goal tracking component that spawned this goal
	UPROPERTY(BlueprintReadOnly, Category = "Goal System")
	UGoalTrackingComponent* OwningGoalTracker;

	//Boolean for tracking if object is a sub goal.  Set when Initialize is called 
	UPROPERTY(BlueprintReadOnly, Category = "Goal System")
	bool bIsSubGoal;

	//Called when all objectives are complete.  Will tell owning goal tracker to update goal data as well 
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Goal System")
    virtual void CompleteGoal();
	
};
