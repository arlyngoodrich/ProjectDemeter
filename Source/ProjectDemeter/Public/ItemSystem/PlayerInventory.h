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
	int32 ActionIndex;

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
	void SetItemInSlot(int32 ActionIndex,FItemData Item);

	UFUNCTION(BlueprintCallable,Category="Inventory")
	bool GetItemFromSlot(int32 ActionSlotIndex,FItemData& Item);
	
protected:
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxActionSlots;

	//Array of Inventory Slot Data.  Array is not in index order. 
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Inventory")
	TArray<FInventorySlotData> ActionSlots;

	UFUNCTION()
	void OnRep_ActionSlotsUpdate() const;


private:

	void RefreshSlots();

	bool GetSlotIndex(int32 ActionIndex, int32&ArrayIndex);

	
};
