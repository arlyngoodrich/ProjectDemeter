#include "GoalSystem/UPickupGoal.h"
#include "Core/Logs_C.h"



UPickupGoal::UPickupGoal()
{

}


void UPickupGoal::Initialize(AActor* OwningActor)
{
    Super::Initalize(AActor* OwningActor);

    //SetInventoryReferences();
}


/*
void UPickupGoal::SetInventoryReference()
    {
        if(OwningActor == nullptr){return;}

        TrackedInventory = FindComponentByClass<UInventoryComponent>();
        if(TrackedInventory == nullptr)
        {
            //Log Error
        }
        else
        {
            TrackedInventory->OnItemAddedDelegate.AddDynamic(this,&UPickupGoal::OnItemAddedToInventory)
        }
    }
*/

/*
void  UPickupGoal::OnItemAddedToInventory(FItemData AddedItem)
{
    if(AddedItem.ItemClass == PickupItemClass)
    {
        CompleteGoal()
    }
}

*/

