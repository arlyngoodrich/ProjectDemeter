// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionSensorComponent.generated.h"


class UInteractableObjectComponent;
class APlayerController;

UCLASS( ClassGroup=(InteractionSystem), blueprintable, meta=(BlueprintSpawnableComponent) )
class PROJECTDEMETER_API UInteractionSensorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionSensorComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction System")
	bool bInteractableObjectInView;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction System")
	UInteractableObjectComponent* InteractableObjectComponentInView;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction System")
	float InteractionDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDrawDebug;

	bool bShouldCheckForInteractable;

	APlayerController* OwningController;

	AActor* ActorInView;

	void Initalize();

	UFUNCTION(BlueprintCallable, Category = "Interaction System")
	void Interact();

	void TriggerInteraction(UInteractableObjectComponent* ComponentInView);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TriggerInteraction(UInteractableObjectComponent* ComponentInView);
	bool Server_TriggerInteraction_Validate(UInteractableObjectComponent* ComponentInView);
	void Server_TriggerInteraction_Implementation(UInteractableObjectComponent* ComponentInView);

	UFUNCTION(BlueprintCallable, Category = "Interaction System")
	void ToggleInteraction(bool bShouldCheckForInteraction);

	UFUNCTION()
	void InteractionCheckLoop();

	UFUNCTION()
	bool GetHitActorInView(AActor*& HitActor);

	UFUNCTION()
	bool GetInteractableComponent(AActor* HitActor, UInteractableObjectComponent*& HitActorInterableObjectComponent);
		
};
