#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GoalSystem/GoalObjectBase.h"
#include "DropOffGoal.generated.h"

/**
 * Objective for dropping off an item of a specific type. Objective complete once item is dropped off.
 */

UCLASS(blueprintable)
class PROJECTDEMETER_API UDropOffGoal : public UGoalObjectBase
{
	GENERATED_BODY()

public:
	
   UDropOffGoal();

   virtual void Initialize(AActor* OwningActor) override;

   //Should be set by object creating this goal
   //UPROPERTY()
   //AActor* TargetDropOffActor;

//protected:

   //tracked inventory component to bind to watch for object dropoff
   //class UInventoryComponent* TrackedInventory;

   //tracked item type
   //TODO need to figure out a way to identify unique item types?  Most likely have ItemData set Item class on construct in item data? 
   //TODO need to add delegates to inventory for when an item is added or removed
   //UPROPERTY(BlueprintEditDefaultsOnly, Category = "Goal System")
   // UClass DropOffItemClass

   //UFUNCITON()
   //void OnItemTranferedFromInventory(FItemData AddedItem, UInventoryComponent* TargetInventory);

    //void SetInventoryReference();

	
};