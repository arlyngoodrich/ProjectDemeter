#include "GoalSystem/PickupGoal.h"
#include "ItemSystem/InventoryComponent.h"
#include "Core/Logs_C.h"
#include "ItemSystem/Item.h"


UPickupGoal::UPickupGoal()
{

}


void UPickupGoal::Initialize(AActor* OwningActor,UGoalTrackingComponent* GoalTrackingComponent,bool bSetIsSubGoal)
{
    Super::Initialize(OwningActor,GoalTrackingComponent,bSetIsSubGoal);

    SetInventoryReference();
}



void UPickupGoal::SetInventoryReference()
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



