// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemSystem/ItemData.h"
#include "InventoryComponent.generated.h"

class UInventoryWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(ItemSystem), blueprintable, meta=(BlueprintSpawnableComponent) )
class PROJECTDEMETER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	bool AddItem(FItemData Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	bool RemoveItem(FItemData Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClientFriendly_RemoveItem(FItemData Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	bool ConsumeItem(FItemData Item, AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void  ClientFriendly_ConsumeItem(FItemData Item, AActor* TargetActor);

	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetMaxInventorySlots() const;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxItems;

	UPROPERTY(EditDefaultsOnly, Category= "Inventory")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_InventoryUpdate, Category = "Inventory")
	TArray<FItemData> Inventory;

	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	UInventoryWidget* InventoryWidget;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveItem(FItemData Item);
	bool Server_RemoveItem_Validate(FItemData Item);
	void Server_RemoveItem_Implementation(FItemData Item);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ConsumeItem(FItemData Item, AActor* TargetActor);
	bool Server_ConsumeItem_Validate(FItemData Item, AActor* TargetActor);
	void Server_ConsumeItem_Implementation(FItemData Item, AActor* TargetActor);

	UFUNCTION()
	void OnRep_InventoryUpdate() const;

private:

	//Create and adds widget to viewport 
	void CreateInventoryWidget();
	
	bool FindFirstIndexOfItem(FItemData Item, int32& Index);

		
};
