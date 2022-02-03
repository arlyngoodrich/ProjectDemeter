#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GoalSystem/GoalObjectBase.h"
#include "ItemSystem/Item.h"
#include "DropOffGoal.generated.h"

/**
 * Objective for dropping off an item of a specific type. Objective complete once item is dropped off.
 */

struct FItemData;

UCLASS(blueprintable)
class PROJECTDEMETER_API UDropOffGoal : public UGoalObjectBase
{
	GENERATED_BODY()

public:
	
   UDropOffGoal();

   virtual void Initialize(AActor* OwningActor) override;

   //Should be set by object creating this goal
   UPROPERTY()
   AActor* TargetDropOffActor;

//protected:

   //tracked inventory component to bind to watch for object drop off
   UPROPERTY()
   class UInventoryComponent* TrackedInventory;

   //tracked item type
   //TODO need to add delegates to inventory for when an item is added or removed
   UPROPERTY(EditDefaultsOnly, Category = "Goal System")
	TSubclassOf<AItem> DropOffItemClass;

    UFUNCTION()
    void OnItemTransferredFromInventory(FItemData TransferredItem, UInventoryComponent* TargetInventory);

	void SetInventoryReference();

	
};