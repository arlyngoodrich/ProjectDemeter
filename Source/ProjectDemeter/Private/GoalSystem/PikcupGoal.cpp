#include "GoalSystem/PickupGoal.h"
#include "ItemSystem/InventoryComponent.h"
#include "Core/Logs_C.h"
#include "ItemSystem/Item.h"


UPickupGoal::UPickupGoal()
{

}


void UPickupGoal::Initialize(AActor* OwningPlayer,UGoalTrackingComponent* GoalTrackingComponent, const bool bSetIsSubGoal,
                             const FText DisplayNameText, const FText DisplayDescriptionText)
{
    Super::Initialize(OwningPlayer,GoalTrackingComponent,bSetIsSubGoal,DisplayNameText, DisplayDescriptionText);

    SetInventoryReference();
}



void UPickupGoal::SetInventoryReference()
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
        TrackedInventory->OnItemAddedToInventoryDelegate.AddDynamic(this,&UPickupGoal::OnItemAddedToInventory);
    }
}




void  UPickupGoal::OnItemAddedToInventory(FItemData AddedItem)
{
    if (AddedItem.ItemClass == PickupItemClass)
    {
        OnItemPickedUpDelegate.Broadcast(AddedItem);
        CompleteGoal();
    }
}

void UPickupGoal::CompleteGoal()
{
    Super::CompleteGoal();

    TrackedInventory->OnItemAddedToInventoryDelegate.RemoveDynamic(this,&UPickupGoal::OnItemAddedToInventory);
}


