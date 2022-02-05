// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_C.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPawnPossessed);

/**
 * 
 */
UCLASS()
class PROJECTDEMETER_API APlayerController_C : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable,Category="Controller")
	FOnPawnPossessed OnPawnPossessedDelegate;

protected:

	virtual void OnPossess(APawn* InPawn) override;
	
};
