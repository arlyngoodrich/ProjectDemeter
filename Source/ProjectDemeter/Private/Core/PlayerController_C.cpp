// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerController_C.h"

void APlayerController_C::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OnPawnPossessedDelegate.Broadcast();
}
