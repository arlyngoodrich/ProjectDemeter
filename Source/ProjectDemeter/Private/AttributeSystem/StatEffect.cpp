// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/StatEffect.h"
#include "Attributesystem/BaseStatComponent.h"
#include "Core/Logs_C.h"

//UE4 Includes
#include "GameFramework/Controller.h"

UStatEffect::UStatEffect()
{
	TargetComponentClass = nullptr;
}

void UStatEffect::InitalizeEffect(AActor* TargetActor, AController* InstigatingController)
{
	if (InstigatingController == nullptr || TargetActor == nullptr)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("Attempted to create %s stat effect with eith Controlled or Target Component Null"), *GetClass()->GetName());

		MarkPendingKill();
		return;
	}

	if (InstigatingController->GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("Attempted to created %s stat effect when instigator is not authority"), *GetClass()->GetName());

		MarkPendingKill();
		return;
	}

	UBaseStatComponent* TargetComponent;
	if(GetTargetComponent(TargetActor, TargetComponent))
	{
		TriggerEffect(TargetComponent);
	}
	else
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("%s could not find target componet class %s on target actor class %s"), 
			*GetClass()->GetName(), 
			*TargetComponentClass->GetName(), 
			*TargetActor->GetClass()->GetName()
		);
		
		MarkPendingKill();
		return;
	}

}

bool UStatEffect::GetTargetComponent(AActor* TargetActor, UBaseStatComponent*& TargetComponent)
{

	TArray<UBaseStatComponent*> StatComponents;
	TargetActor->GetComponents<UBaseStatComponent>(StatComponents);

	for (int32 i = 0; i < StatComponents.Num(); i++)
	{
		if (StatComponents[i]->GetClass() == TargetComponentClass)
		{
			TargetComponent = StatComponents[i];
			return true;
		}
	}

	return false;
}

void UStatEffect::TriggerEffect(UBaseStatComponent* TargetComponent)
{
	if (TargetComponent == nullptr) 
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("%s could not triggere effect because target component was null"), *GetClass()->GetName());

		MarkPendingKill();
		return; 
	}

	TargetComponent->EffectStat(StatEffectAmount);
	UE_LOG(LogAttributeSystem,Log,TEXT("%s applied effect on %s"),*GetName(),*TargetComponent->GetName())


	MarkPendingKill();

}
