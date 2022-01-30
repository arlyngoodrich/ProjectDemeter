// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSystem/ItemData.h"
#include "Item.generated.h"

class UInteractableObjectComponent;

UCLASS()
class PROJECTDEMETER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Item data used in the inventory
	UPROPERTY(EditDefaultsOnly, Category = "Item Data")
	FItemData ItemData;

	//Component that handles interaction with the player
	UPROPERTY(VisibleAnywhere, Category= "Components")
	UInteractableObjectComponent* InteractionComponent;

	//Called on being play.  Handles binding to delegates and specifically on interaction triggered.  
	void Initialize();

	//Called Interaction Component delegate.
	UFUNCTION()
	void OnInteraction(AActor* InstigatingActor);

	//Used to add the item into an inventory.  Expected use is to be called after interaction.  
	void AddToTargetInventory(const AActor* TargetActor);

};
