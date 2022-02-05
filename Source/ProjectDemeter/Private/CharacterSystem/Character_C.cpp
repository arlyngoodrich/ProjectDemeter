// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSystem/Character_C.h"

// Sets default values
ACharacter_C::ACharacter_C(const FObjectInitializer& ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacter_C::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacter_C::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp,Log,TEXT("%s possessed by %s"),*GetName(),*NewController->GetName())
	OnCharacterPossessedDelegate.Broadcast(NewController);
}

// Called every frame
void ACharacter_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacter_C::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
