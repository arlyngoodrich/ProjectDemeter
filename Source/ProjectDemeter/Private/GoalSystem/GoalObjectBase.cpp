#include "GoalSystem/GoalObjectBase.h"
#include "Core/Logs_C.h"
#include "Core/PlayerController_C.h"
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

void UGoalObjectBase::BP_Initialize(AActor* OwningPlayer, UGoalTrackingComponent* GoalTrackingComponent,
	FText DisplayNameText, FText DisplayDescriptionText)
{
	Initialize(OwningPlayer,GoalTrackingComponent,false,DisplayNameText,DisplayDescriptionText);
}


void UGoalObjectBase::Initialize(AActor* OwningPlayer,UGoalTrackingComponent* GoalTrackingComponent,bool bSetIsSubGoal,
	 FText DisplayNameText, FText DisplayDescriptionText)
{

	
    if(OwningPlayer == nullptr)
    {
        UE_LOG(LogGoalSystem,Error,TEXT("%s goal class was given null owning player"),*GetClass()->GetName())
        MarkPendingKill();
        return;
    }

    if(OwningPlayer->GetLocalRole()<ROLE_Authority)
    {
        UE_LOG(LogGoalSystem,Error,TEXT("%s goal class attempted to be initalized with a non-authoritve player"),*GetClass()->GetName())
        MarkPendingKill();
        return;
    }

    OwningActor = OwningPlayer;
	OwningGoalTracker = GoalTrackingComponent;
	bIsSubGoal = bSetIsSubGoal;
	GoalData.GoalGUID = FGuid::NewGuid();
	GoalData.GoalDisplayName = DisplayNameText;
	GoalData.GoalDescription = DisplayDescriptionText;

	APlayerController_C* PlayerController = Cast<APlayerController_C>(OwningActor);
	if(PlayerController)
	{
		PlayerController->OnPawnPossessedDelegate.AddDynamic(this,&UGoalObjectBase::ResetCharacterReferences);
	}
	
}

void UGoalObjectBase::CompleteGoal()
{
	GoalData.bGoalCompleted = true;
	if(bIsSubGoal==false)
	{
		OwningGoalTracker->OnGoalDataUpdate(GoalData);
	}
}

void UGoalObjectBase::ResetCharacterReferences()
{
	
}

bool UGoalObjectBase::GetInventoryComponentFromOwner(UInventoryComponent*& InventoryComponent) const
{
	if (OwningActor == nullptr) { return false; }

	AActor* ActorToSearch;

	APlayerController* PlayerController = Cast<APlayerController>(OwningActor);
	if (PlayerController)
	{
		ActorToSearch = PlayerController->GetCharacter();
		if(ActorToSearch == nullptr)
		{return false;}
	}
	else
	{
		ActorToSearch = OwningActor;
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



