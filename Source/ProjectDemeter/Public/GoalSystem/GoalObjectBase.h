// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GoalObjectBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGoalCompleted);

class APlayerController;
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
    FName GoalName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Goal System")
    FText GoalDescription;

    UPROPERTY(BlueprintReadOnly, Category = "Goal System")
    bool bHasGoalBeenCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Goal System")
    FOnGoalCompleted OnGoalCompletedDelegate;

    virtual void Initalize(APlayerController* PlayerOwningGoal);

protected:

    UPROPERTY(BlueprintReadOnly, Category = "Goal System")
    APlayerController* OwningPlayer;

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Goal System")
    void CompleteGoal();

};
