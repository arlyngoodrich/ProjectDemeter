// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemSystem/ItemData.h"
#include "InventoryComponent.generated.h"

class AItem;
class UInventoryWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAddedToInventory,FItemData,NewItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved,FItemData,ItemRemoved);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemTransfered,FItemData,Item,class UInventoryComponent*,ReceivingInventory);

UCLASS( ClassGroup=(ItemSystem), blueprintable, meta=(BlueprintSpawnableComponent) )
class PROJECTDEMETER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	//Called when an item is added or removed.  Use case is to update the inventory UI
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	//Called when an item is added to the inventory
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnItemAddedToInventory OnItemAddedToInventoryDelegate;

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnItemTransfered OnItemTransferredDelegate;

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnItemRemoved OnItemRemovedDelegate;

	//Authority only method called to add item to inventory.  Must have a valid GUID to be added to inventory 
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	bool AddItem(FItemData Item);

	//Creates new item and initialize remaining pieces of Item Data 
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	bool AddItemType(TSubclassOf<AItem> NewItemType);

	//Creates new item but parent class will be nullptr.  Should only be used for testing.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	bool AddAbstractItem(FItemData ItemData, UClass* Class, FItemData& UpdatedItemData);

	//Authority only method called to remove an item type from the inventory. 
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	bool RemoveItemType(TSubclassOf<class AItem> ItemClass);

	//Authority only method called to remove a specific item from the inventory.  The item must have a valid GUID. 
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly, Category="Inventory")
	bool RemoveItem(FItemData Item);

	//Client friendly version of remove item.  Will call a RPC if owner is not an authority.  Items must have a valid
	//GUID
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClientFriendly_RemoveItem(FItemData Item);

	//Authority only method to transfer items between inventories.  Items must have a valid GUID.  
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Inventory")
	bool TransferItem(FItemData Item, UInventoryComponent* ReceivingInventory);

	//Client friendly version to transfer items between inventories.  Item must have a valid GUID. 
	UFUNCTION(BlueprintCallable ,Category="Inventory")
	void ClientFriendly_TransferItem(FItemData Item,UInventoryComponent* ReceivingInventory);

	//Authority only method to consume item.  Item must have valid GUID. 
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	bool ConsumeItem(FItemData Item, AActor* TargetActor);

	//Client friendly method to consume item.  Item must have valid GUID. 
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void  ClientFriendly_ConsumeItem(FItemData Item, AActor* TargetActor);

	//Checks if an item can be added to an inventory.  Will check local version of inventory.  
	UFUNCTION(BlueprintPure, Category="Inventory")
	bool CanItemBeAdded(FItemData Item) const;

	//Checks local version of inventory for an item GUID.
	UFUNCTION(BlueprintPure, Category="Inventory")
	bool IsItemInInventory(FItemData Item);

	//Checks local version of inventory for an item class.
	UFUNCTION(BlueprintPure, Category="Inventory")
	bool IsItemTypeInInventory(TSubclassOf<AItem> ItemClass);

	//Returns the max slot the inventory has
	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetMaxInventorySlots() const;
	


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Max amount of items the inventory may hold
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxItems;

	//Widget class that the inventory should display 
	UPROPERTY(EditDefaultsOnly, Category= "Inventory")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	//Array of items contained in the inventory 
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_InventoryUpdate, Category = "Inventory")
	TArray<FItemData> Inventory;

	//Pointer to inventory widget
	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	UInventoryWidget* InventoryWidget;

	//Checks to see if owning actor has a player controller
	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	bool bIsOwnedByPlayer;

	//Player controller of owning actor.  Will return null if not owned by player.
	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	APlayerController* OwningPlayer;

	//Server RPC to remove item.  Must have valid GUID
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveItem(FItemData Item);
	bool Server_RemoveItem_Validate(FItemData Item);
	void Server_RemoveItem_Implementation(FItemData Item);

	//Server RPC to consume item.  Must have valid GUID
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ConsumeItem(FItemData Item, AActor* TargetActor);
	bool Server_ConsumeItem_Validate(FItemData Item, AActor* TargetActor);
	void Server_ConsumeItem_Implementation(FItemData Item, AActor* TargetActor);

	//Server RPC to Transfer Item.  Must have valid GUID.
	UFUNCTION(Server,Reliable,WithValidation)
	void Server_TransferItem(FItemData Item,UInventoryComponent* TargetInventory);
	bool Server_TransferItem_Validate(FItemData Item,UInventoryComponent* TargetInventory);
	void Server_TransferItem_Implementation(FItemData Item,UInventoryComponent* TargetInventory);

	//OnRep function for when inventory is updated.  Calls OnInventoryUpdateDelegate as well
	UFUNCTION()
	void OnRep_InventoryUpdate() const;

	//Helper function called in BeginPlay to set owning player
	void SetOwningPlayer();

private:

	//Create and adds widget to viewport 
	void CreateInventoryWidget(APlayerController* PlayerController);

	//Helper function to remove an item at index.  Used by RemoveItem and RemoveItemType
	bool RemoveItemAtIndex(int32 ItemIndex);

	//Finds first index of an item in inventory by item class. Will return false if item not found.
	bool FindFirstIndexOfItemType(const UClass* ItemClass, int32& Index);

	//Finds index of an item in an inventory by GUID.  Will return false if item not found. 
	bool FindItemIndex(FGuid ItemGUID, int32& Index);
		
};
