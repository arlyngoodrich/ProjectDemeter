// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/OneTimeValueChange.h"
#include "AttributeSystem/BaseStatComponent.h"
#include "Core/Logs_C.h"

bool UOneTimeValueChange::TriggerEffect()
{
	if(Super::TriggerEffect())
	{
		TargetComponent->EffectStat(StatEffectAmount);
		UE_LOG(LogAttributeSystem, Log, TEXT("%s applied effect on %s"), *GetName(), *TargetComponent->GetName());
		return true;
	}
	else
	{
		return false;
	}
}
