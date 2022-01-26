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

	UPROPERTY(BlueprintAssignable, Category = "Stat Info")
	FOnCurrentValueChange OnCurrentValueChange;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentValueChange, Category = "Stat Info")
	float CurrentValue;

	UPROPERTY(EditDefaultsOnly, Category = "Stat Info")
	float MaxValue;

	UFUNCTION()
	virtual void OnRep_CurrentValueChange();

		
};
