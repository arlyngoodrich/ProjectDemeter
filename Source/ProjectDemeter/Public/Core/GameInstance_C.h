// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstance_C.generated.h"

/**
 * 
 */
UCLASS(blueprintable)
class PROJECTDEMETER_API UGameInstance_C : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "Story Data")
	FString StoryName;

	
};
