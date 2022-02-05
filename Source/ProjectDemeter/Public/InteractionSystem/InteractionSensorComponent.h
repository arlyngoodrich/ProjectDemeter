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

	//Tracks if there is an interactable in view.  True if yes, False if no. 
	UPROPERTY(BlueprintReadOnly, Category = "Interaction System")
	bool bInteractableObjectInView;

	//Return a pointer to the Interactable Object Component in view.  Will be null if one is not in view. 
	UPROPERTY(BlueprintReadOnly, Category = "Interaction System")
	UInteractableObjectComponent* InteractableObjectComponentInView;

	//Distance that the line trace will check for actors with interactable object components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction System")
	float InteractionDistance;

	//Whether or not the a debug line should be drawn with the line trace
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDrawDebug;

	//Used to toggle interaction on an off.  Expected used would be if a menu is open. 
	UPROPERTY(BlueprintReadOnly, Category="Interaction System")
	bool bShouldCheckForInteractable;

	/*
	*Reference to the owning controller of the interaction sensor.  Used to get camera view and set as a property so
	*not set every tick
	*/
	UPROPERTY()
	APlayerController* OwningController;

	//Used to in by Trigger Interaction Method
	UPROPERTY()
	AActor* ActorInView;

	//Called on begin play to set reference and kick off the interaction check loop
	UFUNCTION()
	void Initialize();

	/*Called by player input to interact with an object if it's in view.  The player will see the interaction text and
	 *object in focus.  If they trigger the interact action without an actor in view, nothing will happen.  
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction System")
	void Interact();

	//Internal method to handle interaction called by the player
	void TriggerInteraction(UInteractableObjectComponent* ComponentInView);

	//RPC for clients to request server to interact with an object 
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TriggerInteraction(UInteractableObjectComponent* ComponentInView);
	bool Server_TriggerInteraction_Validate(UInteractableObjectComponent* ComponentInView);
	void Server_TriggerInteraction_Implementation(UInteractableObjectComponent* ComponentInView);

	//Used to turn off the interaction loop.  Expected use if for when menus are open. 
	UFUNCTION(BlueprintCallable, Category = "Interaction System")
	void ToggleInteraction(bool bShouldCheckForInteraction);

	//Main method for checking for actors with interactable objects 
	UFUNCTION()
	void InteractionCheckLoop();

	//Helper function to get the actor from line trace
	UFUNCTION()
	bool GetHitActorInView(AActor*& HitActor);

	//Helper function that checks for an interactable object component on an actor 
	UFUNCTION()
	bool GetInteractableComponent(AActor* HitActor, UInteractableObjectComponent*& HitActorInteractableObjectComponent);
		
};
