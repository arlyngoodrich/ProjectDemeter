// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSystem/StaminaComponent_C.h"

// Sets default values for this component's properties
UStaminaComponent_C::UStaminaComponent_C()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStaminaComponent_C::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UStaminaComponent_C::OnGaitUpdate(EGait NewGait)
{
}


// Called every frame
void UStaminaComponent_C::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

