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

	UPROPERTY(EditDefaultsOnly, Category = "Item Data")
	FItemData ItemData;

	UPROPERTY(VisibleAnywhere)
	UInteractableObjectComponent* InteractionComponent;

	void Initalize();

	UFUNCTION()
	void OnInteraction(AActor* InstigatingActor);


};
