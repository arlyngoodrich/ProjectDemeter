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
   UPROPERTY(BlueprintReadOnly, Category = "Goal System")
   FGoalData GoalData;

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Goal System", DisplayName ="Initialize")
	void BP_Initialize(AActor* OwningPlayer, UGoalTrackingComponent* GoalTrackingComponent, FText DisplayNameText,
	                   FText DisplayDescriptionText);
	
	/**
	 * @brief Called by spawning object
	 * @param OwningPlayer should usually be the player controller but can also be an actor for testing
	 * @param GoalTrackingComponent the goal component that is tracking this goal.  Should be the component on the
	 * actor or player controller.
	 * @param bSetIsSubGoal used in native code for creating sub codes.  Should always be false when used in blueprints.
	 * @param DisplayNameText will be used to set the GoalData and displayed on the UI
	 * @param DisplayDescriptionText will be used to set the GoalData and displayed on the UI
	 */
	virtual void Initialize(AActor* OwningPlayer, UGoalTrackingComponent* GoalTrackingComponent, bool bSetIsSubGoal,
	                        FText DisplayNameText, FText DisplayDescriptionText);

	

protected:

	//Will typically be a player controller but may be an actor if used for testing
    UPROPERTY(BlueprintReadOnly, Category = "Goal System")
    AActor* OwningActor;

	//Goal tracking component that spawned this goal
	UPROPERTY(BlueprintReadOnly, Category = "Goal System")
	UGoalTrackingComponent* OwningGoalTracker;

	//Boolean for tracking if object is a sub goal.  Set when Initialize is called 
	UPROPERTY(BlueprintReadOnly, Category = "Goal System")
	bool bIsSubGoal;

	//Called when all objectives are complete.  Will tell owning goal tracker to update goal data as well 
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Goal System")
    virtual void CompleteGoal();
	
	bool GetInventoryComponentFromOwner(class UInventoryComponent*& InventoryComponent) const;
	
};
