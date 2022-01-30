#include "GoalSystem/GoalObjectBase.h"
#include "Core/Logs_C.h"



// Sets default values for this component's properties
UGoalObjectBase::UGoalObjectBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	 
	// ...
}



void UGoalObjectBase::Initialize(AActor* OwningActor)
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
	GoalData.GoalGUID = FGuid::NewGuid();
}

void UGoalObjectBase::CompleteGoal()
{
}



