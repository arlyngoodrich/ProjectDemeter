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
            //Log Error
        }
        else
        {
            //TrackedInventory->OnItemAddedDelegate.AddDynamic(this,&UPickupGoal::OnItemAddedToInventory);
        }
    }



void  UPickupGoal::OnItemAddedToInventory(FItemData AddedItem)
{
   // if(AddedItem.ItemClass == PickupItemClass)
  //  {
      //  CompleteGoal()
  //  }
}



