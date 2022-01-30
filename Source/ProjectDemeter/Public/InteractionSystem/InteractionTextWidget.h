// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDEMETER_API UInteractionTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category="Interaction System")
	FText InteractionText;
};
