#include "GoalSystem/GoalObjectBase.h"
#include "Core/Logs_C.h"
#include "GameFramework/Character.h"
#include "GoalSystem/GoalTrackingComponent.h"
#include "ItemSystem/InventoryComponent.h"


// Sets default values for this component's properties
UGoalObjectBase::UGoalObjectBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	 
	// ...
}



void UGoalObjectBase::Initialize(AActor* OwningActor,UGoalTrackingComponent* GoalTrackingComponent,bool bSetIsSubGoal)
{
    if(OwningActor == nullptr)
    {
        UE_LOG(LogGoalSystem,Error,TEXT("%s goal class was given null owning player"),*GetClass()->GetName())
        MarkPendingKill();
        return;
    }

    if(OwningActor->GetLocalRole()<ROLE_Authority)
    {
        UE_LOG(LogGoalSystem,Error,TEXT("%s goal class attempted to be initalized with a non-authoritve player"),*GetClass()->GetName())
        MarkPendingKill();
        return;
    }

    OwningPlayer = OwningActor;
	OwningGoalTracker = GoalTrackingComponent;
	bIsSubGoal = bSetIsSubGoal;
	GoalData.GoalGUID = FGuid::NewGuid();
}

void UGoalObjectBase::CompleteGoal()
{
	GoalData.bGoalCompleted = true;
	if(bIsSubGoal==false)
	{
		OwningGoalTracker->OnGoalDataUpdate(GoalData);
	}
}

bool UGoalObjectBase::GetInventoryComponentFromOwner(UInventoryComponent*& InventoryComponent) const
{
	if (OwningPlayer == nullptr) { return false; }

	AActor* ActorToSearch;

	APlayerController* PlayerController = Cast<APlayerController>(OwningPlayer);
	if (PlayerController)
	{
		ActorToSearch = PlayerController->GetCharacter();
	}
	else
	{
		ActorToSearch = OwningPlayer;
	}

	InventoryComponent = ActorToSearch->FindComponentByClass<UInventoryComponent>();

	if (InventoryComponent)
	{
		return true;
	}
	else
	{
		return false;
	}
}



