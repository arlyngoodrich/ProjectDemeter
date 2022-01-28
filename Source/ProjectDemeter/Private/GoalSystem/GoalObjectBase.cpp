#include "GoalObjectBase.generated.h"
#include "Core/Logs_C.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UGoalObjectBase::UGoalObjectBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

void Initalize(APlayerController* PlayerOwningGoal)
{
    if(PlayerOwningGoal == nullptr)
    {

        MarkPendingKill();
        return;
    }

    OwningPlayer = PlayerOwningGoal;

}

void UGoalObjectBase::CompleteGoal()
{
    /
}