// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent_C.generated.h"

UENUM(BlueprintType)
enum class EGait : uint8 {
	EG_None			UMETA(DisplayName = "None"),
	EG_Walk			UMETA(DisplayName = "Walk"),
	EG_Run			UMETA(DisplayName = "Run"),
	EG_Sprint		UMETA(DisplayName = "Sprint")
};


UCLASS( ClassGroup=(Custom), blueprintable, meta=(BlueprintSpawnableComponent) )
class PROJECTDEMETER_API UStaminaComponent_C : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent_C();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Stamina")
	void OnGaitUpdate(EGait NewGait);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
