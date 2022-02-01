#include "GoalSystem/DropOffGoal.h"
#include "Core/Logs_C.h"



UDropOffGoal::UDropOffGoal()
{

}


void UDropOffGoal::Initialize(AActor* OwningActor)
{
    Super::Initalize(AActor* OwningActor);
    
    //SetInventoryReference();


}


/*
void UDropOffGoal::SetInventoryReference()
    {
        if(OwningActor == nullptr){return;}

        TrackedInventory = FindComponentByClass<UInventoryComponent>();
        if(TrackedInventory == nullptr)
        {
            //Log Error
        }
        else
        {
            TrackedInventory->OnItemTransferedDelegate.AddDynamic(this,&UDropOffGoal::OnItemTranferedFromInventory)
        }
    }
*/

/*
void UDropOffGoal::OnItemTranferedFromInventory(FItemData TransferedItem, UInventoryComponent* TargetInventory)
{

    if(TransferedItem.Class == DropOffItemClass && TargetInventory->GetOwner() == TargetDropOffActor)
    {
        CompletedGoal();
    }

}
*/