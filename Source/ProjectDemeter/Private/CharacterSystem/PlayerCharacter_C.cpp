// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSystem/PlayerCharacter_C.h"

//Custom Invludes
#include "CharacterSystem/PlayerCharMovementComponent_C.h"
//#include "LogFiles.h"

//UE4 Includes
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APlayerCharacter_C::APlayerCharacter_C(const FObjectInitializer& ObjectInitializer)


	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerCharMovementComponent_C>(ACharacter_C::CharacterMovementComponentName))

{

	// ==== Default Values ===== =====
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// ==== Components ===== =====

	//Add Spring Arm for Camera
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->bEnableCameraRotationLag = true;
	SpringArmComp->bInheritPitch = false;

	//Spring Arm default values
	SpringArmComp->SetRelativeLocation(FVector(20.f, 0.f, 85.f));
	SpringArmComp->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArmComp->TargetArmLength = 150.f;

	//Add Camera Component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);


	//Default Movement Values
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = DefaultCrouchSpeed;

}

// Called when the game starts or when spawned
void APlayerCharacter_C::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlayerCharacter_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ==== Replicated Variables ===== =====

void APlayerCharacter_C::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter_C, bWantsToSprint);
	DOREPLIFETIME(APlayerCharacter_C, bIsSprinting);
	DOREPLIFETIME(APlayerCharacter_C, SprintSpeedMuliplyer)


}




// ==== Movement ===== =====


// Called to bind functionality to input
void APlayerCharacter_C::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter_C::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter_C::MoveRight);
	PlayerInputComponent->BindAxis("TurnRight", this, &APlayerCharacter_C::TurnRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter_C::LookUp);


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter_C::Jump);
	PlayerInputComponent->BindAction("ToggleCrouch", IE_Pressed, this, &APlayerCharacter_C::ToggleCrouch);
	PlayerInputComponent->BindAction("ToggleSprint", IE_Pressed, this, &APlayerCharacter_C::SetWantsToSprint);

}

bool APlayerCharacter_C::GetWantsToSprint()
{
	// resets sprint if not moving
	if (GetVelocity().Size() == 0 && bWantsToSprint == true)
	{
		SetWantsToSprint();
	}

	return bWantsToSprint;
}

float APlayerCharacter_C::GetSprintSpeedModifier() { return SprintSpeedMuliplyer; }

void APlayerCharacter_C::SetSprintSpeedModifer(float NewSprintModifer)
{
	if (HasAuthority())
	{
		SprintSpeedMuliplyer = NewSprintModifer;
	}

}

float APlayerCharacter_C::GetDefaultWalkSpeed() { return DefaultWalkSpeed; }


//Forward and Back
void APlayerCharacter_C::MoveForward(float Value)
{

	AddMovementInput(GetActorForwardVector() * Value);

}


//Right and Left
void APlayerCharacter_C::MoveRight(float Value)
{

	AddMovementInput(GetActorRightVector() * Value);

}

void APlayerCharacter_C::TurnRight(float Value)
{

	AddControllerYawInput(Value);

}


void APlayerCharacter_C::LookUp(float Value)
{

	if (CameraComp == nullptr) { return; }

	AddControllerPitchInput(Value);


}

void APlayerCharacter_C::Jump()
{
	Super::Jump();
}

void APlayerCharacter_C::ToggleCrouch()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
		if (bWantsToSprint == true) { SetWantsToSprint(); }
	}
}



void APlayerCharacter_C::SetWantsToSprint()
{

	if (GetLocalRole() < ROLE_Authority)
	{
		// Is NOT Authority
		Server_SetWantsToSprint();
	}
	else
	{
		//Is Authority
		if (bWantsToSprint == false)
		{
			bWantsToSprint = true;
			WantsToSprint_OnSprintStart.Broadcast();
			//UE_LOG(LogTPCharacter, Log, TEXT("Character wants to sprint"))

		}
		else
		{
			bWantsToSprint = false;
			WantsToNotSprint_OnSptrintStop.Broadcast();
			//UE_LOG(LogTPCharacter, Log, TEXT("Character wants to stop sprint"))
		}
	}
}


bool APlayerCharacter_C::Server_SetWantsToSprint_Validate()
{
	return true;
}

void APlayerCharacter_C::Server_SetWantsToSprint_Implementation()
{
	SetWantsToSprint();

}

float APlayerCharacter_C::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage_DamageTaken.Broadcast(DamageAmount);

	return DamageAmount;
}

