// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSystem/InventoryComponent.h"
#include "PlayerInventory.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlotData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 ActionSlot;

	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	bool bHasItem;

	UPROPERTY()
	FGuid ItemGUID;

	

};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionSlotUpdate);

/**
 * 
 */
UCLASS()
class PROJECTDEMETER_API UPlayerInventory : public UInventoryComponent
{
	GENERATED_BODY()

public:
	
	UPlayerInventory();

	UPROPERTY(BlueprintAssignable,Category="Inventory")
	FOnActionSlotUpdate OnActionSlotUpdateDelegate;
	
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Inventory")
	void SetItemInSlot(int32 ActionSlot,FItemData Item);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void UseItemInActionSlot(int32 ActionSlot);
	
	UFUNCTION(BlueprintCallable,Category="Inventory")
	bool GetItemFromSlot(int32 ActionSlot,FItemData& Item);

	
protected:
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxActionSlots;

	//Array of Inventory Slot Data.  Array is not in index order. 
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Inventory")
	TArray<FInventorySlotData> ActionSlots;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_ActionSlotsUpdate() const;

	UFUNCTION()
	void CheckForActionItemRemoval(FItemData Item);

	UFUNCTION()
	void RemoveItemFromSlot(int32 ActionSlot);

private:

	void CreateSlots();

	bool GetSlotIndex(int32 ActionSlot, int32&ArrayIndex);

	FGuid NullGUID;

	
};
