// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GoalSystem/GoalData.h"
#include "GoalObjectBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGoalCompleted);

/**
 * 
 */

UCLASS(blueprintable)
class PROJECTDEMETER_API UGoalObjectBase : public UObject
{
	GENERATED_BODY()

public:
	
    UGoalObjectBase();

   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Goal System")
   FGoalData GoalData;

    UPROPERTY(BlueprintAssignable, Category = "Goal System")
    FOnGoalCompleted OnGoalCompletedDelegate;

    virtual void Initialize(AActor* OwningActor);

protected:

    UPROPERTY(BlueprintReadOnly, Category = "Goal System")
    AActor* OwningPlayer;

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Goal System")
    void CompleteGoal();

	FGuid GoalGUID;
	
};
