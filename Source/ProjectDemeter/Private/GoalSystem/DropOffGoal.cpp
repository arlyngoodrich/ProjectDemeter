#include "GoalSystem/DropOffGoal.h"
#include "Core/Logs_C.h"
#include "ItemSystem/InventoryComponent.h"


UDropOffGoal::UDropOffGoal()
{

}


void UDropOffGoal::Initialize(AActor* OwningActor)
{
    Super::Initialize(OwningActor);
    
    SetInventoryReference();


}



void UDropOffGoal::SetInventoryReference()
    {
        if(OwningPlayer == nullptr){return;}

        TrackedInventory = OwningPlayer->FindComponentByClass<UInventoryComponent>();
        if(TrackedInventory == nullptr)
        {
            UE_LOG(LogGoalSystem,Error,TEXT("%s could not find invnetory on owning player"),*GetClass()->GetName())
        }
        else
        {
           //TrackedInventory->OnItemTransferredDelegate.AddDynamic(this,&UDropOffGoal::OnItemTranferredFromInventory)
        }
    }



void UDropOffGoal::OnItemTransferredFromInventory(FItemData TransferredItem, UInventoryComponent* TargetInventory)
{

    if(TransferredItem.ItemClass == DropOffItemClass && TargetInventory->GetOwner() == TargetDropOffActor)
    {
        CompleteGoal();
    }

}
