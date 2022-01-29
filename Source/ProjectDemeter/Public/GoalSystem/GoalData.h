// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoalData.generated.h"


USTRUCT(BlueprintType)
struct FGoalData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal Info")
	FName GoalDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal Info")
	FText GoalDescription;

	UPROPERTY(BlueprintReadOnly, Category = "Goal Info")
	bool bGoalCompleted;

};