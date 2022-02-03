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
	void SetupPickupGoal();
	void SetupDropOffGoal();
	virtual void Initialize(AActor* OwningActor) override;

	UPROPERTY(BlueprintReadWrite,Category="Goal System", Meta = (ExposeOnSpawn=true))
	AActor* DeliveryTarget;

protected:

	UPROPERTY(EditDefaultsOnly,Category="Goal System")
	TSubclassOf<AItem> ItemDeliveryClass;
	
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
