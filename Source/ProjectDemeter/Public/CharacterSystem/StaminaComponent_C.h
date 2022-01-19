// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent_C.generated.h"

UENUM(BlueprintType)
enum class EGait : uint8 {
	EG_None			UMETA(DisplayName = "None"),
	EG_Walk			UMETA(DisplayName = "Walk"),
	EG_Run			UMETA(DisplayName = "Run"),
	EG_Sprint		UMETA(DisplayName = "Sprint")
};


UCLASS( ClassGroup=(Custom), blueprintable, meta=(BlueprintSpawnableComponent) )
class PROJECTDEMETER_API UStaminaComponent_C : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent_C();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float MaxStamina;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float SprintStaminaDrain;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float StaminaRegenerationAmount;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	float CurrentStamina;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Stamina")
	void OnGaitUpdate(EGait NewGait);

	void OnSprintStart();

	void OnRunStart();

	void OnWalkStart();

	void OnNoneStart();

	FTimerHandle SprintStaminaDrainTimer;

	FTimerHandle StaminaRegenerationTimer;

private:

	void RegenerateStamina();

	void DrainSprintStamina();

	void UpdateStamina(float StaminaDelta);

		
};
