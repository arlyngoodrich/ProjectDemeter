// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemSystem/ItemData.h"
#include "InventoryComponent.generated.h"

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

<<<<<<< HEAD
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	bool RemoveItem(FItemData Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClientFriendly_RemoveItem(FItemData Item);
=======
>>>>>>> parent of a1585f7 (revert)

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
<<<<<<< HEAD
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxItems;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_InventoryUpdate, Category = "Inventory")
	TArray<FItemData> Inventory;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveItem(FItemData Item);
	bool Server_RemoveItem_Validate(FItemData Item);
	void Server_RemoveItem_Implementation(FItemData Item);

	UFUNCTION()
	void OnRep_InventoryUpdate();

private:

	bool FindFirstIndexOfItem(FItemData Item, int32& Index);
=======
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_InventoryUpdate, Category = "Inventory")
	TArray<FItemData> Inventory;

	UFUNCTION()
	void OnRep_InventoryUpdate();
>>>>>>> parent of a1585f7 (revert)
		
};
