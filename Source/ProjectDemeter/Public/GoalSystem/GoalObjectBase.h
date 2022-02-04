// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GoalSystem/GoalData.h"
#include "GoalObjectBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoalDataUpdate,FGoalData,GoalData);

class UGoalTrackingComponent;

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

	UPROPERTY(BlueprintAssignable,Category="GoalSystem")
	FOnGoalDataUpdate OnGoalDataUpdateDelegate;

	
   virtual void Initialize(AActor* OwningActor,UGoalTrackingComponent* GoalTrackingComponent,bool bSetIsSubGoal);

protected:

    UPROPERTY(BlueprintReadOnly, Category = "Goal System")
    AActor* OwningPlayer;

	UPROPERTY(BlueprintReadOnly, Category = "Goal System")
	UGoalTrackingComponent* OwningGoalTracker;

	UPROPERTY(BlueprintReadOnly, Category = "Goal System")
	bool bIsSubGoal;

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Goal System")
    void CompleteGoal();
	
};
