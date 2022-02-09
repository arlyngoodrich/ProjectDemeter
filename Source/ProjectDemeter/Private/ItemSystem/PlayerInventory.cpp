// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSystem/PlayerInventory.h"
#include "Core/Logs_C.h"
#include "Net/UnrealNetwork.h"

UPlayerInventory::UPlayerInventory()
{
	CreateSlots();
	NullGUID = FGuid::NewGuid();
}

void UPlayerInventory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerInventory, ActionSlots, COND_None,REPNOTIFY_Always);

}

void UPlayerInventory::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwnerRole()==ROLE_Authority)
	{
		this->OnItemRemovedDelegate.AddDynamic(this,&UPlayerInventory::CheckForActionItemRemoval);	
	}

}

void UPlayerInventory::SetItemInSlot(const int32 ActionSlot, const FItemData Item)
{

	if(Item.ItemGUID.IsValid()==false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Attempted to place item with invalid GUID into %s action slot %d"),
		       *GetOwner()->GetName(), ActionSlot)
		return;
	}
	
	int32 ArrayIndex;
	if(GetSlotIndex(ActionSlot,ArrayIndex))
	{
		ActionSlots[ArrayIndex].ItemGUID = Item.ItemGUID;
		ActionSlots[ArrayIndex].bHasItem = true;

		OnRep_ActionSlotsUpdate();

		UE_LOG(LogInventorySystem,Log,TEXT("%s item is in %d action slot"),*Item.DisplayName.ToString(),ActionSlot);
	}
	else
	{
		UE_LOG(LogInventorySystem,Log,TEXT("%d is not a valid slot for %s"),ActionSlot,*GetOwner()->GetName())
	}
	
}

void UPlayerInventory::UseItemInActionSlot(const int32 ActionSlot)
{
	FItemData ActionSlotItem;
	if(GetItemFromSlot(ActionSlot,ActionSlotItem))
	{
		ClientFriendly_UseItem(ActionSlotItem,GetOwner());
		//If item successfully used by inventory, CheckForActionItemRemoval will see it be removed from the inventory
		//and remove it from the actions slot
	}
}

void UPlayerInventory::CheckForActionItemRemoval(const FItemData Item)
{
	if(GetOwnerRole()<=ROLE_Authority){return;}
	
	for (int i = 0; i < ActionSlots.Num(); ++i)
	{
		if(ActionSlots[i].ItemGUID == Item.ItemGUID)
		{
			RemoveItemFromSlot(ActionSlots[i].ActionSlot);
		}

	}
}

void UPlayerInventory::RemoveItemFromSlot(const int32 ActionSlot)
{
	if(GetOwnerRole()<=ROLE_Authority){return;}

	int32 Index;
	if(GetSlotIndex(ActionSlot,Index))
	{
		ActionSlots[Index].bHasItem = false;
		ActionSlots[Index].ItemGUID = NullGUID;
		OnRep_ActionSlotsUpdate();
	}
}

bool UPlayerInventory::GetItemFromSlot(const int32 ActionSlot, FItemData& Item)
{
	int32 ArrayIndex;
	if(GetSlotIndex(ActionSlot,ArrayIndex))
	{
		if(ActionSlots[ArrayIndex].bHasItem == false)
		{
			return false;
		}
		
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



void UPlayerInventory::CreateSlots()
{
	ActionSlots.Empty();
	
	for (int i = 1; i == MaxActionSlots; ++i)
	{
		FInventorySlotData NewSlot;
		NewSlot.ActionSlot = i;
		NewSlot.bHasItem = false;
		NewSlot.ItemGUID = NullGUID;

		ActionSlots.Add(NewSlot);
	}
}

bool UPlayerInventory::GetSlotIndex(const int32 ActionSlot, int32& ArrayIndex)
{
	for (int i = 0; i < ActionSlots.Num(); ++i)
	{
		if(ActionSlots[i].ActionSlot == ActionSlot)
		{
			ArrayIndex = i;
			return true;
		}
	}

	return false;
}
