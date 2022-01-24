// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSystem/ItemData.h"
#include "Item.generated.h"

UCLASS()
class PROJECTDEMETER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Item Data")
	FItemData ItemData;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Interaction System")
	void PickupItem(AActor* InstigatingActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
