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

	virtual void Initialize(AActor* OwningPlayer,UGoalTrackingComponent* GoalTrackingComponent,bool bSetIsSubGoal,
							 FText DisplayNameText, FText DisplayDescriptionText) override;

	UPROPERTY()
	FOnITemDroppedOff OnITemDroppedOffDelegate;

	//Should be set by object creating this goal
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Goal System", meta=(ExposeOnSpawn))
	AActor* TargetDropOffActor;

	//tracked item type
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Goal System", meta=(ExposeOnSpawn))
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