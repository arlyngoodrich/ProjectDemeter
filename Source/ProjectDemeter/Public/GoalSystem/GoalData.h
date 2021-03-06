// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoalData.generated.h"


USTRUCT(BlueprintType)
struct FGoalData
{
	GENERATED_USTRUCT_BODY()

	FGuid GoalGUID;

	//Name text for Goal Tracking UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal Info")
	FText GoalDisplayName;

	//Description text for Goal Tracking UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal Info")
	FText GoalDescription;

	//Read only for if goal is completed or not 
	UPROPERTY(BlueprintReadOnly, Category = "Goal Info")
	bool bGoalCompleted;

};