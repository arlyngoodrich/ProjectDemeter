// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSystem/HealthComponent_C.h"

// Sets default values for this component's properties
UHealthComponent_C::UHealthComponent_C()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.f;

	// ...
}


// Called when the game starts
void UHealthComponent_C::BeginPlay()
{
	Super::BeginPlay();

	Initalize();
	
}

void UHealthComponent_C::Initalize()
{
	MyOwner = GetOwner();
	CurrentHealth = MaxHealth;
	MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent_C::OnOwnerTakeDamage);	
}

void UHealthComponent_C::OnOwnerTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("Damage taken: %f"), Damage);
	AdjustHealth(-Damage);
}

void UHealthComponent_C::AdjustHealth(float HealthDelta)
{
	float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + HealthDelta, 0.f, MaxHealth);

	if (CurrentHealth != OldHealth)
	{
		UE_LOG(LogTemp,Log, TEXT("New Health: %f"), CurrentHealth)
	}
}





