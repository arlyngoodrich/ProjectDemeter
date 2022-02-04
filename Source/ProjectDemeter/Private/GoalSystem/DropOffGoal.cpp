#include "GoalSystem/DropOffGoal.h"
#include "Core/Logs_C.h"
#include "ItemSystem/InventoryComponent.h"
#include "ItemSystem/Item.h"


UDropOffGoal::UDropOffGoal()
{

}


void UDropOffGoal::Initialize(AActor* OwningPlayer,UGoalTrackingComponent* GoalTrackingComponent,bool bSetIsSubGoal)
{
    Super::Initialize(OwningPlayer,GoalTrackingComponent,bSetIsSubGoal);
    
    SetInventoryReference();
}



void UDropOffGoal::SetInventoryReference()
{
    if (OwningActor == nullptr) { return; }

    GetInventoryComponentFromOwner(TrackedInventory);
        
    if (TrackedInventory == nullptr)
    {
        UE_LOG(LogGoalSystem, Error, TEXT("%s could not find invnetory on owning player"), *GetClass()->GetName())
    }
    else
    {
        TrackedInventory->OnItemTransferredDelegate.AddDynamic(this, &UDropOffGoal::OnItemTransferredFromInventory);
    }
}

// ReSharper disable once CppParameterMayBeConst
// ReSharper disable once CppParameterMayBeConstPtrOrRef
void UDropOffGoal::OnItemTransferredFromInventory(FItemData TransferredItem, UInventoryComponent* TargetInventory)
{
    if (TransferredItem.ItemClass == DropOffItemClass && TargetInventory->GetOwner() == TargetDropOffActor)
    {
        OnITemDroppedOffDelegate.Broadcast(TransferredItem,TargetInventory);
        CompleteGoal();
    }
}

void UDropOffGoal::CompleteGoal()
{
    Super::CompleteGoal();

    TrackedInventory->OnItemTransferredDelegate.RemoveDynamic(this, &UDropOffGoal::OnItemTransferredFromInventory);
}
