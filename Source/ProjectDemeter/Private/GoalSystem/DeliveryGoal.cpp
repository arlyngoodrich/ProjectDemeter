// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalSystem/DeliveryGoal.h"
#include "Core/Logs_C.h"
#include "GoalSystem/DropOffGoal.h"
#include "GoalSystem/PickupGoal.h"
#include "ItemSystem/ItemData.h"
#include "ItemSystem/InventoryComponent.h"



void UDeliveryGoal::Initialize(AActor* OwningActor)
{
	Super::Initialize(OwningActor);
	SetInventoryReference();
	SetupPickupGoal();
	SetupDropOffGoal();

}

void UDeliveryGoal::SetInventoryReference()
{
	if(OwningPlayer == nullptr){return;}

	TrackedInventory = OwningPlayer->FindComponentByClass<UInventoryComponent>();
	if(TrackedInventory == nullptr)
	{
		UE_LOG(LogGoalSystem, Error, TEXT("%s goal could not set inventory reference on owning player"),
			   *GetClass()->GetName());
	}
	else
	{
		TrackedInventory->OnItemRemovedDelegate.AddDynamic(this,&UDeliveryGoal::OnItemRemoved);
	}
}

void UDeliveryGoal::SetupPickupGoal()
{
	PickupGoal = NewObject<UPickupGoal>(this,UPickupGoal::StaticClass());
	PickupGoal->Initialize(OwningPlayer);
	PickupGoal->PickupItemClass = ItemDeliveryClass;
	PickupGoal->OnItemPickedUpDelegate.AddDynamic(this,&UDeliveryGoal::OnItemPickedUp);
}

void UDeliveryGoal::SetupDropOffGoal()
{
	DropOffGoal = NewObject<UDropOffGoal>(this,UDropOffGoal::StaticClass());
	DropOffGoal->Initialize(OwningPlayer);
	DropOffGoal->TargetDropOffActor = DeliveryTarget;
	DropOffGoal->DropOffItemClass = ItemDeliveryClass;
	DropOffGoal->OnITemDroppedOffDelegate.AddDynamic(this,&UDeliveryGoal::OnItemDroppedOff);
}

void UDeliveryGoal::OnItemPickedUp(FItemData Item)
{

	if(ItemGUID.IsValid())
	{
		//Add item to Item GUID to make sure we're tracking it 
		ItemGUID = Item.ItemGUID;
	}
	else
	{
		UE_LOG(LogGoalSystem,Error,TEXT("%s was given invalid item GUID for deliver"),*GetClass()->GetName())
	}
}


void UDeliveryGoal::OnItemRemoved(FItemData Item)
{

	if(Item.ItemGUID == ItemGUID)
	{
		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer,this,&UDeliveryGoal::CheckToResetPickupGoals,
			1.f,false,0.f);
	}
	
}



void UDeliveryGoal::OnItemDroppedOff(FItemData Item, UInventoryComponent* ReceivingInventory)
{
	if(Item.ItemGUID == ItemGUID)
	{
		bDropOffComplete = true;
		CompleteGoal();
	}
	else
	{
		UE_LOG(LogGoalSystem,Error,TEXT("%s cannot complete if item dropped GUID does not match"),*GetClass()->GetName())
	}

}

void UDeliveryGoal::CheckToResetPickupGoals()
{
	if(bDropOffComplete == false)
	{
		UE_LOG(LogGoalSystem,Log,TEXT("%s had tracked item dropped of at non-target inventory.  Restarting pickup goal"),*GetName())
		PickupGoal->OnItemPickedUpDelegate.RemoveAll(this);
		SetupPickupGoal();
	}
}
