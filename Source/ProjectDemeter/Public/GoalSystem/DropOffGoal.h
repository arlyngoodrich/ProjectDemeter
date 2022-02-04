#pragma once

#include "CoreMinimal.h"
#include "GoalSystem/GoalObjectBase.h"
#include "ItemSystem/ItemData.h"
#include "DropOffGoal.generated.h"

class AItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnITemDroppedOff,struct FItemData,Item,class UInventoryComponent*, ReceivingInventory);

/**
 * Objective for dropping off an item of a specific type. Objective complete once item is dropped off.
 */



UCLASS(blueprintable)
class PROJECTDEMETER_API UDropOffGoal : public UGoalObjectBase
{
	GENERATED_BODY()

public:
	
   UDropOffGoal();

   virtual void Initialize(AActor* OwningActor,UGoalTrackingComponent* GoalTrackingComponent,bool bSetIsSubGoal) override;

   //Should be set by object creating this goal
   UPROPERTY()
   AActor* TargetDropOffActor;

	UPROPERTY()
	FOnITemDroppedOff OnITemDroppedOffDelegate;

	//tracked item type
	UPROPERTY(EditDefaultsOnly, Category = "Goal System")
	TSubclassOf<AItem> DropOffItemClass;

//protected:

   //tracked inventory component to bind to watch for object drop off
   UPROPERTY()
   class UInventoryComponent* TrackedInventory;

    UFUNCTION()
    void OnItemTransferredFromInventory(FItemData TransferredItem, UInventoryComponent* TargetInventory);

	void SetInventoryReference();

	virtual void CompleteGoal() override;
	
};