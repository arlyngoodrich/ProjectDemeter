// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSystem/InventoryWidget.h"
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
}

void UInventoryWidget::Internal_RefreshInventory()
{
	BP_RefreshInventoryUI();
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Internal_RefreshInventory();
}

