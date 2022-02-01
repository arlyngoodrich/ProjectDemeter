#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GoalSystem/GoalObjectBase.h"
#include "GoalObjectBase.generated.h"

/**
 * 
 */

UCLASS(blueprintable)
class PROJECTDEMETER_API UPickupGoal : public UGoalObjectBase
{
	GENERATED_BODY()

public:
	
   UPickupGoal();

   virtual void Initialize(AActor* OwningActor) override;

	
};