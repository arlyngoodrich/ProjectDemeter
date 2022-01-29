// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoalTrackingComponent.generated.h"

class UGoalObjectBase;
class APlayerController;


UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class PROJECTDEMETER_API UGoalTrackingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoalTrackingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Initialize();

	UPROPERTY(BlueprintReadOnly, Category= "References")
	APlayerController* OwningPlayer;
	
	UPROPERTY(BlueprintReadOnly,Category = "Goal Info")
	TArray<UGoalObjectBase*> ActiveGoals;

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly, Category="Goal System", DisplayName = "Add Goal")
	void BP_AddGoal(TSubclassOf<UGoalObjectBase> GoalToAdd);

	virtual void Internal_AddGoal(TSubclassOf<UGoalObjectBase> GoalToAdd);

	virtual void Internal_RemoveGoal(UGoalObjectBase* GoalToRemove);
	

	


		
};
