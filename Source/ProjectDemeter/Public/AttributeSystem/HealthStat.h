// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/BaseStatComponent.h"
#include "HealthStat.generated.h"



UCLASS( ClassGroup=(AttributeSystem), meta=(BlueprintSpawnableComponent) )
class PROJECTDEMETER_API UHealthStat : public UBaseStatComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthStat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    //Used to bind to component owner to register damage events 
   	UFUNCTION()
	virtual void OnOwnerTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

};