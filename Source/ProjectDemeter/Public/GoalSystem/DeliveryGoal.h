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
UCLASS(blueprintable)
class PROJECTDEMETER_API UDeliveryGoal : public UGoalObjectBase
{
	GENERATED_BODY()

public:

	//Called by object creating goal
	virtual void Initialize(AActor* OwningPlayer,UGoalTrackingComponent* GoalTrackingComponent,bool bSetIsSubGoal,
                            FText DisplayNameText, FText DisplayDescriptionText) override;

	//Should be set by the creating object 
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Goal System", Meta = (ExposeOnSpawn=true))
	AActor* DeliveryTarget;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Goal System",Meta = (ExposeOnSpawn=true))
	TSubclassOf<AItem> ItemDeliveryClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Goal System",Meta = (ExposeOnSpawn=true))
	FText PickupGoalDisplayText;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Goal System",Meta = (ExposeOnSpawn=true))
	FText DropOffGoalDisplayText;
	
protected:

	void SetupPickupGoal(bool bIsReplacement);
	
	void SetupDropOffGoal();

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
