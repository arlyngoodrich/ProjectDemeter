// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableObjectComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionTriggeredDelegate, AActor*, InstigatingActor);


UCLASS( ClassGroup=(InteractionSystem), blueprintable, meta=(BlueprintSpawnableComponent) )
class PROJECTDEMETER_API UInteractableObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableObjectComponent();

	//Called by Interactable Object Sensor on player 
	void Interact(AActor* InstigatingActor);

	/*
	 *Called by Interactable Object Sensor on player.  Sets if a player has the object in view.  Will highlight all
	 *mesh objects when in view. 
	*/
	void ToggleFocus(bool bNewIsInFocus);

	//Delegate triggered when the player call interact while the owning actor is in view
	UPROPERTY(BlueprintAssignable, Category = "Interaction System")
	FInteractionTriggeredDelegate OnInteractionTriggered;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Text to display while the object is in focus
	UPROPERTY(EditDefaultsOnly, Category="Interaction System")
	FText InteractionText;

	//Configuration for if the object should highlight or not. 
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System")
	bool bShouldOutline;

	/*
	 *Configuration for which stencil value the outline should use.  Check the Outline Material Post Process Material
	 *To see which colors are available.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System")
	int32 OutlineStencilValue;

	//Boolean value for if the owning object is in focus by the player.  True if yes, false if no. 
	UPROPERTY(BlueprintReadOnly, Category = "Interaction System")
	bool bIsInFocus;

	//Adds or removes outline to all mesh objects on the owning object. 
	void ToggleOutline(bool bStartOutline) const;
		
};
