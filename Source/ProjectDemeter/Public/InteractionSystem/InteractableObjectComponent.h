// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableObjectComponent.generated.h"


UCLASS( ClassGroup=(InteractionSystem), blueprintable, meta=(BlueprintSpawnableComponent) )
class PROJECTDEMETER_API UInteractableObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableObjectComponent();

	void Interact();

	void ToggleFocus(bool bNewIsInFocus);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction System", DisplayName = "On Focus Start")
	void BP_OnStartFocus();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction System", DisplayName = "On End Focus")
	void BP_OnEndFocus();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction System", DisplayName = "On Interaction Triggered")
	void BP_OnInteractionTriggered();
	
	UPROPERTY(BlueprintReadOnly, Category = "Interaction System")
	bool bIsInFocus;




		
};
