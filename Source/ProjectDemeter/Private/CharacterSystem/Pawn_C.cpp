// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSystem/Pawn_C.h"

// Sets default values
APawn_C::APawn_C()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void APawn_C::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawn_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APawn_C::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

