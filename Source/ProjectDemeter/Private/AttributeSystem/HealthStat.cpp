// Fill out your copyright notice in the Description page of Project Settings.

#include "AttributeSystem/HealthStat.h"
#include "Core/Logs_C.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"

UHealthStat::UHealthStat()
{
    //Default Values
    MaxValue = 100.f;
    BaseRegenerationAmount = 5.f;
    bShouldRegenerate = true;
    RegenerationDelay = 5.f;
}


void UHealthStat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthStat, bHasDied);

}

float UHealthStat::GetCurrentHealth() { return CurrentValue; }

float UHealthStat::GetMaxHealth(){return MaxValue;}

float UHealthStat::GetRegenerationAmount(){return BaseRegenerationAmount;}

// Called when the game starts
void UHealthStat::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}

void UHealthStat::Initialize()
{
    Super::Initialize();

    GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthStat::OnOwnerTakeDamage);
}

void UHealthStat::OnOwnerTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    ChangeCurrentValue(-Damage);
    StartRegenerationDelay();
}

void UHealthStat::OnRep_CurrentValueChange()
{
    //Check to see if owner has died
    if(CurrentValue == 0)
    {
        bHasDied = true;
        OnRep_HasDied();
    }

    Super::OnRep_CurrentValueChange();
}

void UHealthStat::OnRep_HasDied()
{
    UE_LOG(LogAttributeSystem,Log,TEXT("%s has died"),*GetOwner()->GetName());

    //Trun off regeneration
    bShouldRegenerate = false;
    ToggleRegeneration(false);

    //Broadcast death
    OnDeathDelegate.Broadcast();
}

 void UHealthStat::StartRegenerationDelay()
 {
     //Don't start timer if the owner has died
     if(bHasDied == true){return;}

    //Check to see if timer exisist.  If yes, then clear
    if(GetWorld()->GetTimerManager().TimerExists(RegenerationDelayTimer))
    {
        GetWorld()->GetTimerManager().ClearTimer(RegenerationDelayTimer);
    }

    GetWorld()->GetTimerManager().SetTimer(RegenerationDelayTimer, this, &UHealthStat::StartRegeneration, RegenerationDelay, false);

 }

 void UHealthStat::StartRegeneration()
 {
    ToggleRegeneration(true);
 }