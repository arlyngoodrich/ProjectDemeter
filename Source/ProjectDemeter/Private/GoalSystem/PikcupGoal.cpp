#include "GoalSystem/PickupGoal.h"
#include "ItemSystem/InventoryComponent.h"
#include "Core/Logs_C.h"



UPickupGoal::UPickupGoal()
{

}


void UPickupGoal::Initialize(AActor* OwningActor)
{
    Super::Initialize(OwningActor);

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



void  UPickupGoal::OnItemAddedToInventory(const FItemData AddedItem)
{
    if (AddedItem.ItemClass == PickupItemClass)
    {
        CompleteGoal();
    }
}



