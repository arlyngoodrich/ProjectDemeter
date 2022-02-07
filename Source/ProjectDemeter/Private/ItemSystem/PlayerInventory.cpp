// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSystem/PlayerInventory.h"
#include "Core/Logs_C.h"
#include "Net/UnrealNetwork.h"

UPlayerInventory::UPlayerInventory()
{
	RefreshSlots();
}

void UPlayerInventory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerInventory, ActionSlots, COND_None,REPNOTIFY_Always);

}

void UPlayerInventory::SetItemInSlot(const int32 ActionIndex, const FItemData Item)
{

	if(Item.ItemGUID.IsValid()==false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Attempted to place item with invalid GUID into %s action slot %d"),
		       *GetOwner()->GetName(), ActionIndex)
		return;
	}
	
	int32 ArrayIndex;
	if(GetSlotIndex(ActionIndex,ArrayIndex))
	{
		ActionSlots[ArrayIndex].ItemGUID = Item.ItemGUID;
		ActionSlots[ArrayIndex].bHasItem = true;

		OnRep_ActionSlotsUpdate();

		UE_LOG(LogInventorySystem,Log,TEXT("%s item is in %d action slot"),*Item.DisplayName.ToString(),ActionIndex);
	}
	else
	{
		UE_LOG(LogInventorySystem,Log,TEXT("%d is not a valid slot for %s"),ActionIndex,*GetOwner()->GetName())
	}
	
}

bool UPlayerInventory::GetItemFromSlot(const int32 ActionSlotIndex, FItemData& Item)
{
	int32 ArrayIndex;
	if(GetSlotIndex(ActionSlotIndex,ArrayIndex))
	{
		const FGuid ItemGuid = ActionSlots[ArrayIndex].ItemGUID;
		
		int32 InventoryIndex;
		if(FindItemIndex(ItemGuid,InventoryIndex))
		{
			Item = Inventory[InventoryIndex];
			return true;
		}
	}

	return false;
}

void UPlayerInventory::OnRep_ActionSlotsUpdate() const
{
	OnActionSlotUpdateDelegate.Broadcast();
}

void UPlayerInventory::RefreshSlots()
{

	ActionSlots.Empty();
	
	for (int i = 1; i == MaxActionSlots; ++i)
	{
		FInventorySlotData NewSlot;
		NewSlot.ActionIndex = i;
		NewSlot.bHasItem = false;

		ActionSlots.Add(NewSlot);
	}
}

bool UPlayerInventory::GetSlotIndex(const int32 ActionIndex, int32& ArrayIndex)
{
	for (int i = 0; i < ActionSlots.Num(); ++i)
	{
		if(ActionSlots[i].ActionIndex == ActionIndex)
		{
			ArrayIndex = i;
			return true;
		}
	}

	return false;
}
