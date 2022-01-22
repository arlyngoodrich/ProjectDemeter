// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSystem/HealthComponent_C.h"
#include "Core/Logs_C.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent_C::UHealthComponent_C()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.f;

	// ...
}


void UHealthComponent_C::GetHealthStats(float& OutCurrentHealth, float& OutMaxHealth)
{
	OutCurrentHealth = CurrentHealth;
	OutMaxHealth = MaxHealth;
}

// Called when the game starts
void UHealthComponent_C::BeginPlay()
{
	Super::BeginPlay();

	Initalize();
	
}

void UHealthComponent_C::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent_C, CurrentHealth);
	DOREPLIFETIME(UHealthComponent_C, bHasDied);

}

void UHealthComponent_C::Initalize()
{
	MyOwner = GetOwner();
	CurrentHealth = MaxHealth;
	MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent_C::OnOwnerTakeDamage);	
	bHasDied = false;
}

void UHealthComponent_C::OnOwnerTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogHealth, Log, TEXT("%s recieved %f damage"), *GetOwner()->GetName(), Damage);
	AdjustHealth(-Damage);
}

void UHealthComponent_C::OnRep_HealthChange()
{
	BP_OnHealthChange(CurrentHealth);
}

void UHealthComponent_C::OnRep_bHasDied()
{
	UE_LOG(LogHealth, Log, TEXT("%s has died"),*GetOwner()->GetName())
	BP_OnDied();
}

void UHealthComponent_C::AdjustHealth(float HealthDelta)
{
	float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + HealthDelta, 0.f, MaxHealth);

	if (CurrentHealth != OldHealth)
	{
		OnRep_HealthChange();
		UE_LOG(LogHealth, Log, TEXT("%s changed to %f "), *GetOwner()->GetName(), CurrentHealth)

		if (CurrentHealth < MaxHealth && bShouldRegenerate)
		{
			ToggleHealthRegen(true);
		}

	}

	if (CurrentHealth == MaxHealth)
	{
		ToggleHealthRegen(false);
	}

	if (CurrentHealth == 0)
	{
		bHasDied = true;
		OnRep_bHasDied();
		ToggleHealthRegen(false);
		bShouldRegenerate = false;
	}
}

void UHealthComponent_C::ToggleHealthRegen(bool bShouldtSartRegen)
{
	
	if (bShouldtSartRegen)
	{
		//Do nothing if timer is already active
		if (GetWorld()->GetTimerManager().IsTimerActive(HealthRegenTimer)) { return; }

		GetWorld()->GetTimerManager().SetTimer(HealthRegenTimer, this, &UHealthComponent_C::RegenHealth, 1.f, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(HealthRegenTimer);
	}
}

void UHealthComponent_C::RegenHealth()
{
	AdjustHealth(BaseRegenerationAmount);
	UE_LOG(LogHealth, Log, TEXT("%s has regenerated %f health"), *GetOwner()->GetName(), BaseRegenerationAmount)
}





