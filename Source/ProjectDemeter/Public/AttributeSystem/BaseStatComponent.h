// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCurrentValueChange);


UCLASS( ClassGroup=(AttributeSystem), meta=(BlueprintSpawnableComponent) )
class PROJECTDEMETER_API UBaseStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseStatComponent();

	UPROPERTY(BlueprintAssignable, Category = "Stat System")
	FOnCurrentValueChange OnCurrentValueChangeDelegate;

	//Must be called on server. Will add to the current sat value
	void EffectStat(float EffectAmount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Initialize();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentValueChange, Category = "Stat Info")
	float CurrentValue;

	UPROPERTY(EditDefaultsOnly, Category = "Stat Info")
	float MaxValue;

	UPROPERTY(EditDefaultsOnly, Category = "Stat Info")
	bool bShouldRegenerate;

	UPROPERTY(EditDefaultsOnly, Category = "Stat Info", meta = (EditCondition = "bShouldRegenerate"))
	float BaseRegenerationAmount;

	UFUNCTION()
	virtual void OnRep_CurrentValueChange();

	//Called on both owning client and server when the current value is updated.  Usefully for driving non-game play needed events like UI updates. 
	UFUNCTION(BlueprintImplementableEvent, Category = "Stat System", DisplayName = "On Current Value Change")
	void BP_OnCurrentValueChange();

	//Called on both owning client and server when the current value is updated.  This is the source implementation so it can be overriden by base classes.
	virtual void OnCurrentValueChange();
	
	//Helper function that triggered when max value is reached
	virtual void MaxValueReached();

	FTimerHandle RegenerationTimer;

	//Delta Amount will be added to current value so use negative numbers if wanting to take away value.  Will only run when component owner has authority
	void ChangeCurrentValue(float DeltaAmount);

	void ToggleRegeneration(bool bShouldStart);

private:

	void RegenerateValue();


};
