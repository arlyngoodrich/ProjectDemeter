// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent_C.generated.h"


UCLASS( ClassGroup=(Custom), blueprintable, meta=(BlueprintSpawnableComponent) )
class PROJECTDEMETER_API UHealthComponent_C : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent_C();

	UFUNCTION(BlueprintPure, Category = "Health")
	void GetHealthStats(float& OutCurrentHealth, float& OutMaxHealth);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Initalize();

	AActor* MyOwner;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health")
	bool bShouldRegenerate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health", meta = (EditCondition = "bShouldRegenerate"))
	float BaseRegenerationAmount;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UFUNCTION()
	virtual void OnOwnerTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:

	void AdjustHealth(float HealthDelta);

	//Will ignore start request if already regenerating
	void ToggleHealthRegen(bool bShouldtSartRegen);

	void RegenHealth();
	
	FTimerHandle HealthRegenTimer;

};
