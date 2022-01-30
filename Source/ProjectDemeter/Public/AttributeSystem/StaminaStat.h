// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/BaseStatComponent.h"
#include "StaminaStat.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaDepleted, bool, bIsDepleted);

//Used to Convert ALS Gait ENUM to C++
UENUM(BlueprintType)
enum class EGait_New : uint8 {
	EG_None			UMETA(DisplayName = "None"),
	EG_Walk			UMETA(DisplayName = "Walk"),
	EG_Run			UMETA(DisplayName = "Run"),
	EG_Sprint		UMETA(DisplayName = "Sprint")
};

UCLASS( ClassGroup=(AttributeSystem), blueprintable, meta=(BlueprintSpawnableComponent) )
class PROJECTDEMETER_API UStaminaStat : public UBaseStatComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaStat();

    UPROPERTY(BlueprintAssignable, Category = "Stamina Stats")
    FOnStaminaDepleted OnStaminaDepletedChangeDelegate;

    UFUNCTION(BlueprintPure, Category = "Stamina Stats")
    float GetCurrentStamina();

    UFUNCTION(BlueprintPure, Category = "Stamina Stats")
    float GetMaxStamina();

protected:

	virtual void BeginPlay() override;

    //How much stamina should be drained by sprinting every second
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina Stats")
    float SprintStaminaDrain;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina Stats")
    float JumptStaminaCost;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina Stats")
    float RollStmainaCost;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaminaDepletedUpdated, Category = "Stamina Stats")
    bool bIsStaminaDepleted;

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Stamina Stats")
    void OnJump();

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Stamina Stats")
    void OnRoll();

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Stamina Stats")
    void OnCustomAction(float StaminaCost);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Stamina Stats")
	void OnGaitUpdate(EGait_New NewGait);

    UFUNCTION()
    virtual void OnSprintStart();

    UFUNCTION()
	virtual void OnRunStart();

    UFUNCTION()
	virtual void OnWalkStart();

    virtual void OnCurrentValueChange() override;

    UFUNCTION()
    virtual void OnRep_StaminaDepletedUpdated(); 

private:

    FTimerHandle SprintStaminaDrainTimer;

   void ToggleSprintStaminaDrain(bool bDrain);

   void DrainSprintStamina();

   bool bIsWalking;
   
};