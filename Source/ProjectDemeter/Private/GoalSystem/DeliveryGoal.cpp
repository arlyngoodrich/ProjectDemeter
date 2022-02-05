// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalSystem/DeliveryGoal.h"
#include "Core/Logs_C.h"
#include "GoalSystem/DropOffGoal.h"
#include "GoalSystem/PickupGoal.h"
#include "ItemSystem/ItemData.h"
#include "ItemSystem/InventoryComponent.h"



void UDeliveryGoal::Initialize(AActor* OwningPlayer,UGoalTrackingComponent* GoalTrackingComponent, const bool bSetIsSubGoal,
                               const FText DisplayNameText, const FText DisplayDescriptionText)
{
	Super::Initialize(OwningPlayer,GoalTrackingComponent,bSetIsSubGoal,DisplayNameText,  DisplayDescriptionText);

	if(OwningPlayer == nullptr) {return;}
	
	SetInventoryReference();
	SetupPickupGoal(false);
	SetupDropOffGoal();
}

void UDeliveryGoal::SetInventoryReference()
{
	if(OwningActor == nullptr){return;}

	GetInventoryComponentFromOwner(TrackedInventory);
	
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

void UDeliveryGoal::SetupPickupGoal(bool bIsReplacement)
{
	PickupGoal = NewObject<UPickupGoal>(this,UPickupGoal::StaticClass());
	PickupGoal->Initialize(OwningActor, OwningGoalTracker,
	                       true, PickupGoalDisplayText, FText());
	PickupGoal->PickupItemClass = ItemDeliveryClass;
	PickupGoal->OnItemPickedUpDelegate.AddDynamic(this,&UDeliveryGoal::OnItemPickedUp);

	FSubGoalData PickupGoalSubData;
	PickupGoalSubData.GoalGUID = PickupGoal->GoalData.GoalGUID;
	PickupGoalSubData.GoalDisplayName = PickupGoalDisplayText;
	PickupGoalSubData.bGoalCompleted = false;

	if(bIsReplacement == false)
	{
		GoalData.SubGoalData.Add(PickupGoalSubData);
		OwningGoalTracker->OnGoalDataUpdate(GoalData);
	}
	else
	{
		GoalData.SubGoalData[0] = PickupGoalSubData;
		OwningGoalTracker->OnGoalDataUpdate(GoalData);
	}
}

void UDeliveryGoal::SetupDropOffGoal()
{
	DropOffGoal = NewObject<UDropOffGoal>(this,UDropOffGoal::StaticClass());
	DropOffGoal->Initialize(OwningActor, OwningGoalTracker,
		true, DropOffGoalDisplayText, FText());
	DropOffGoal->TargetDropOffActor = DeliveryTarget;
	DropOffGoal->DropOffItemClass = ItemDeliveryClass;
	DropOffGoal->OnITemDroppedOffDelegate.AddDynamic(this,&UDeliveryGoal::OnItemDroppedOff);

	FSubGoalData DropOffGoalSubData;
	DropOffGoalSubData.GoalGUID = DropOffGoal->GoalData.GoalGUID;
	DropOffGoalSubData.GoalDisplayName = DropOffGoalDisplayText;
	DropOffGoalSubData.bGoalCompleted = false;

	GoalData.SubGoalData.Add(DropOffGoalSubData);
	OwningGoalTracker->OnGoalDataUpdate(GoalData);

}

void UDeliveryGoal::OnItemPickedUp(FItemData Item)
{

	if(Item.ItemGUID.IsValid())
	{
		//Add item to Item GUID to make sure we're tracking it 
		ItemGUID = Item.ItemGUID;
		GoalData.SubGoalData[0].bGoalCompleted = true;
		OwningGoalTracker->OnGoalDataUpdate(GoalData);
	}
	else
	{
		UE_LOG(LogGoalSystem,Error,TEXT("%s was given invalid item GUID for deliveru"),*GetClass()->GetName())
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
		GoalData.SubGoalData[1].bGoalCompleted = true;

		//Complete goal will update goal data in owning tracker
		//OwningGoalTracker->OnGoalDataUpdate(GoalData);
		
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
		SetupPickupGoal(true);
	}
}

void UDeliveryGoal::ResetCharacterReferences()
{
	Super::ResetCharacterReferences();

	SetInventoryReference();
	
}
