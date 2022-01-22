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

	UFUNCTION(BlueprintPure, Category = "Stamina")
	bool GetIsStaminaDepleted();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina")
	float MaxStamina;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina")
	float StaminaDepletionLevel;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina")
	float StaminaRegenerationAmount;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina")
	float SprintStaminaDrain;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina")
	float JumpStaminaDrain;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina")
	float RollStaminaDrain;

	UPROPERTY(ReplicatedUsing = OnRep_StaminaChange, BlueprintReadOnly, Category = "Stamina")
	float CurrentStamina;

	UPROPERTY(ReplicatedUsing = OnRep_IsDepletedUpdated, BlueprintReadOnly, Category = "Stamina")
	bool bIsStaminaDepleted;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Stamina")
	void OnGaitUpdate(EGait NewGait);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Stamina")
	void OnJump();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Stamina")
	void OnRoll();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Stamina")
	void OnCustomAction(float StaminaCost);

	UFUNCTION(BlueprintImplementableEvent, Category = "Stamina", DisplayName = "On Stamina Depleted")
	void BP_OnStaminaDepleted();

	UFUNCTION(BlueprintImplementableEvent, Category = "Stamina", DisplayName = "On Stamina Regenerated")
	void BP_OnStaminaRegenerated();

	UFUNCTION(BlueprintImplementableEvent, Category = "Stamina", DisplayName = "Local On Stamina Update")
	void BP_OnStaminaUpdate(float UpdatedStamina);

	UFUNCTION()
	void OnRep_StaminaChange();

	UFUNCTION()
	void OnRep_IsDepletedUpdated();

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

	void StartStaminaRegeneration();

	void StopStaminaRegeneration();

	void StartStaminaDrain();

	void StopStaminaDrain();

		
};
