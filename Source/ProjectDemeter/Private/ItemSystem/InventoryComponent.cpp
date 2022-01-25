// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSystem/InventoryComponent.h"
#include "Core/Logs_C.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}




// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Inventory);

}



void UInventoryComponent::OnRep_InventoryUpdate()
{
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::ClientFriendly_RemoveItem(FItemData Item)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		Server_RemoveItem(Item);
	}
	else
	{
		RemoveItem(Item);
	}
}


bool UInventoryComponent::AddItem(FItemData Item)
{

	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogInventorySystem, Log, TEXT("Attempting to add %s item to %s inventory as non-authority"), *Item.DisplayName.ToString(), *GetOwner()->GetName())
			
		return false;
	}


	Inventory.Add(Item);
	UE_LOG(LogInventorySystem, Log, TEXT("%s was added to %s's inventory"), *Item.DisplayName.ToString(), *GetOwner()->GetName());
	
	OnRep_InventoryUpdate();
	
	return true;

}

bool UInventoryComponent::RemoveItem(FItemData Item)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogInventorySystem, Log, TEXT("Attempting to remove %s item from %s inventory as non-authority"), *Item.DisplayName.ToString(), *GetOwner()->GetName())

			return false;
	}

	int32 ItemIndex;
	if (FindFirstIndexOfItem(Item,ItemIndex))
	{
		Inventory.RemoveAt(ItemIndex);
		OnRep_InventoryUpdate();

		UE_LOG(LogInventorySystem, Log, TEXT("%s removed from %s inventory."), *Item.DisplayName.ToString(), *GetOwner()->GetName())
		return true;
	}
	else
	{

		UE_LOG(LogInventorySystem,Log,TEXT("%s not found in %s inventory.  Could not remove item"),*Item.DisplayName.ToString(),*GetOwner()->GetName())
		return false;
	}
}


bool UInventoryComponent::FindFirstIndexOfItem(FItemData Item, int32& Index)
{

	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i].DisplayName == Item.DisplayName)
		{
			Index = i;
			return true;
		}
	}

	return false;
}


bool UInventoryComponent::Server_RemoveItem_Validate(FItemData Item)
{
	return true;
}

void UInventoryComponent::Server_RemoveItem_Implementation(FItemData Item)
{
	RemoveItem(Item);
}