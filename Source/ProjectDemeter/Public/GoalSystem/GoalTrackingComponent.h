// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoalData.h"
#include "Components/ActorComponent.h"
#include "GoalTrackingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGoalDataUpdate);

class UGoalObjectBase;


UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class PROJECTDEMETER_API UGoalTrackingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoalTrackingComponent();

	/**
	 * Called on both server and client when the Goal Data array is updated.  Called when goals are added, removed, or
	 * modified 
	 */
	UPROPERTY(BlueprintAssignable, Category="Goal System")
	FGoalDataUpdate OnGoalDataUpdateDelegate;

	UFUNCTION()
	void OnGoalDataUpdate(FGoalData GoalData);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Called on begin play
	virtual void Initialize();

	//Set during initialization 
	UPROPERTY(BlueprintReadOnly, Category= "References")
	AActor* OwningActor;
	
	//Should only be references from the server.  Is not replicated and will be null on clients.
	TArray<UGoalObjectBase*> ActiveGoals;
	
	//UI Data for active goals will be replicated to clients
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_GoalDataUpdate, Category = "Goal Info")
	TArray<FGoalData> ActiveGoalData;
	
	//Blueprint method only so internal method can be overriden by child classes
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly, Category="Goal System", DisplayName = "Add Goal")
	void BP_AddGoal(TSubclassOf<UGoalObjectBase> GoalToAdd);
	
	/*
	* Method called when the Goal Data array property is replicated to the client.  Also called on server functions when
	* the array is modified natively.  
	*/
	UFUNCTION()
	void OnRep_GoalDataUpdate() const;
	
	/*
	* Internal overridable function to add Goals to tracking.  Will add both the goal object to the server
	* and goal data to the server and client
	*/
	virtual void Internal_AddGoal(TSubclassOf<UGoalObjectBase> GoalToAdd);

	/*
	 *Internal overrideable function to remove goals from tracking.  Will remove the goal object from the server the goal
	 * data from the client. 
	 */
	virtual void Internal_RemoveGoal(UGoalObjectBase* GoalToRemove);

	//Helper function to find the index given a goal GUID
	bool FindGoalDataByGUID(FGuid GUID, int32& OutGoalDataIndex) const;


};
