// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSystem/InventoryWidget.h"

#include "AttributeSystem/StatEffect.h"
#include "Core/Logs_C.h"
#include "ItemSystem/InventoryComponent.h"

void UInventoryWidget::SetUpInventoryWidget(UInventoryComponent* CreatingInventory)
{
	if(CreatingInventory == nullptr)
	{
		UE_LOG(LogInventorySystem,Error,TEXT("%s attempted to initialize with null inventory"),*GetClass()->GetName())
	}
	
	OwningInventory = CreatingInventory;
	NumberOfSlots = OwningInventory->GetMaxInventorySlots();
	OwningInventory->OnInventoryUpdated.AddDynamic(this,&UInventoryWidget::Internal_RefreshInventory);
	OwningInventory->GetOwner()->OnDestroyed.AddDynamic(this,&UInventoryWidget::InternalRemoveWidget);
	
}

void UInventoryWidget::UseItem(FItemData ItemToConsume)
{
		//If yes, then consume it 
		OwningInventory->ClientFriendly_UseItem(ItemToConsume,OwningInventory->GetOwner());
}

void UInventoryWidget::Internal_RefreshInventory()
{
	BP_RefreshInventoryUI();
}

void UInventoryWidget::InternalRemoveWidget(AActor* DestroyedActor)
{
	RemoveFromParent();
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Internal_RefreshInventory();
}

