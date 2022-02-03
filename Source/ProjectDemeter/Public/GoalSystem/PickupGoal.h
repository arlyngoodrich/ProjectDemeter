#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GoalSystem/GoalObjectBase.h"
#include "ItemSystem/ItemData.h"
#include "PickupGoal.generated.h"

/**
 * Objective for picking up an item of a specific type. Objective complete once item is picked up. Should be used as a subtask within larger goals.   
 */

UCLASS(blueprintable)
class PROJECTDEMETER_API UPickupGoal : public UGoalObjectBase
{
	GENERATED_BODY()

public:
	
   UPickupGoal();

   virtual void Initialize(AActor* OwningActor) override;

//protected:

   //tracked inventory component to bind to watch for object pickup
   UPROPERTY()
   class UInventoryComponent* TrackedInventory;

   //tracked item type
   UPROPERTY(EditDefaultsOnly, Category = "Goal System")
   UClass* PickupItemClass;

  UFUNCTION()
  void OnItemAddedToInventory(FItemData AddedItem);

  void SetInventoryReference();

	
};