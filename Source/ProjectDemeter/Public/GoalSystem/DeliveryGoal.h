// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoalSystem/GoalObjectBase.h"
#include "ItemSystem/InventoryComponent.h"
#include "DeliveryGoal.generated.h"


class UPickupGoal;
class UDropOffGoal;

/**
 * 
 */
UCLASS()
class PROJECTDEMETER_API UDeliveryGoal : public UGoalObjectBase
{
	GENERATED_BODY()

public:

	//Called by object creating goal
	virtual void Initialize(AActor* OwningActor,UGoalTrackingComponent* GoalTrackingComponent,bool bSetIsSubGoal) override;

	//Should be set by the creating object 
	UPROPERTY(BlueprintReadWrite,Category="Goal System", Meta = (ExposeOnSpawn=true))
	AActor* DeliveryTarget;

protected:

	void SetupPickupGoal();
	
	void SetupDropOffGoal();

	UPROPERTY(EditDefaultsOnly,Category="Goal System")
	TSubclassOf<AItem> ItemDeliveryClass;

	UPROPERTY(EditDefaultsOnly,Category="Goal System")
	FText PickupGoalDisplayText;

	UPROPERTY(EditDefaultsOnly,Category="Goal System")
	FText DropOffGoalDisplayText;
	
	UPROPERTY()
	UPickupGoal* PickupGoal;

	UPROPERTY()
	UDropOffGoal* DropOffGoal;

	UPROPERTY()
	UInventoryComponent* TrackedInventory;

	//Tracked Item GUID
	UPROPERTY()
	FGuid ItemGUID;

	UFUNCTION()
	void SetInventoryReference();

	UFUNCTION()
	void OnItemRemoved(FItemData Item);
	
	UFUNCTION()
	void OnItemPickedUp(FItemData Item);

	UFUNCTION()
	void OnItemDroppedOff(FItemData Item, UInventoryComponent* ReceivingInventory);

	UFUNCTION()
	void CheckToResetPickupGoals();

	bool bDropOffComplete;
	
};
