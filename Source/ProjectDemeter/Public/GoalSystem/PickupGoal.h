#pragma once

#include "CoreMinimal.h"
#include "GoalTrackingComponent.h"
#include "GoalSystem/GoalObjectBase.h"
#include "ItemSystem/ItemData.h"
#include "PickupGoal.generated.h"

/**
 * Objective for picking up an item of a specific type. Objective complete once item is picked up. Should be used as a subtask within larger goals.   
 */

class AItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemPickedUp,FItemData,Item);


UCLASS(blueprintable)
class PROJECTDEMETER_API UPickupGoal : public UGoalObjectBase
{
	GENERATED_BODY()

public:
	
   UPickupGoal();

   virtual void Initialize(AActor* OwningActor,UGoalTrackingComponent* GoalTrackingComponent,bool bSetIsSubGoal) override;

	UPROPERTY()
	FOnItemPickedUp OnItemPickedUpDelegate;

	//tracked item type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal System", meta = (ExposeOnSpawn = true))
	TSubclassOf<AItem> PickupItemClass;
	
//protected:

   //tracked inventory component to bind to watch for object pickup
	UPROPERTY()
	class UInventoryComponent* TrackedInventory;



	UFUNCTION()
	void OnItemAddedToInventory(FItemData AddedItem);

	void SetInventoryReference();
	
	virtual void CompleteGoal() override;
	
};