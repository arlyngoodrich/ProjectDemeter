// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/BaseStatComponent.h"
#include "HealthStat.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(AttributeSystem), blueprintable, meta=(BlueprintSpawnableComponent) )
class PROJECTDEMETER_API UHealthStat : public UBaseStatComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthStat();

    UPROPERTY(BlueprintAssignable, Category = "Health Stats")
    FOnDeath OnDeathDelegate;

    UFUNCTION(BlueprintPure, Category = "Health Stats")
    float GetCurrentHealth();

    UFUNCTION(BlueprintPure, Category = "Health Stats")
    float GetMaxHealth();

    UFUNCTION(BlueprintPure, Category = "Health Stats")
    float GetRegenerationAmount();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    virtual void Initialize() override;

    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Health Stats")
    float RegenerationDelay;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HasDied, Category =  "Health Stats")
    bool bHasDied;

    //Used to bind to component owner to register damage events 
   	UFUNCTION()
	void OnOwnerTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    virtual void OnCurrentValueChange() override;

    UFUNCTION()
    virtual void OnRep_HasDied();

private:

    FTimerHandle RegenerationDelayTimer;

    //Starts a timer for the durration of the Regeneration Delay property.  Will reset every time function is called. 
    void StartRegenerationDelay();

    void StartRegeneration();

};