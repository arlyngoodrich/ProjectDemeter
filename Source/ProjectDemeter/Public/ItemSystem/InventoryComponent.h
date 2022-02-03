// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemSystem/ItemData.h"
#include "InventoryComponent.generated.h"

class UInventoryWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAddedToInventory,FItemData,NewItem);

UCLASS( ClassGroup=(ItemSystem), blueprintable, meta=(BlueprintSpawnableComponent) )
class PROJECTDEMETER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
	
	FOnItemAddedToInventory OnItemAddedToInventoryDelegate;

	//Called to add item to inventory.  Must have a valid GUID to be added to inventory 
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	bool AddItem(FItemData Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	bool RemoveItemType(TSubclassOf<class AItem> ItemClass);

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly, Category="Inventory")
	bool RemoveItem(FItemData Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClientFriendly_RemoveItem(FItemData Item);
	
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Inventory")
	bool TransferItem(FItemData Item, UInventoryComponent* ReceivingInventory);

	UFUNCTION(BlueprintCallable ,Category="Inventory")
	void ClientFriendly_TransferItem(FItemData Item,UInventoryComponent* ReceivingInventory);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	bool ConsumeItem(FItemData Item, AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void  ClientFriendly_ConsumeItem(FItemData Item, AActor* TargetActor);

	UFUNCTION(BlueprintPure, Category="Inventory")
	bool CanItemBeAdded(FItemData Item) const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	bool IsItemInInventory(FItemData Item);

	UFUNCTION(BlueprintPure, Category="Inventory")
	bool IsItemTypeInInventory(TSubclassOf<AItem> ItemClass);

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

	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	bool bIsOwnedByPlayer;

	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	APlayerController* OwningPlayer;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveItem(FItemData Item);
	bool Server_RemoveItem_Validate(FItemData Item);
	void Server_RemoveItem_Implementation(FItemData Item);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ConsumeItem(FItemData Item, AActor* TargetActor);
	bool Server_ConsumeItem_Validate(FItemData Item, AActor* TargetActor);
	void Server_ConsumeItem_Implementation(FItemData Item, AActor* TargetActor);

	UFUNCTION(Server,Reliable,WithValidation)
	void Server_TransferItem(FItemData Item,UInventoryComponent* TargetInventory);
	bool Server_TransferItem_Validate(FItemData Item,UInventoryComponent* TargetInventory);
	void Server_TransferItem_Implementation(FItemData Item,UInventoryComponent* TargetInventory);
	
	UFUNCTION()
	void OnRep_InventoryUpdate() const;
	
	void SetOwningPlayer();

private:

	//Create and adds widget to viewport 
	void CreateInventoryWidget(APlayerController* PlayerController);

	bool RemoveItemAtIndex(int32 ItemIndex);
	
	bool FindFirstIndexOfItemType(TSubclassOf<class AItem> ItemClass, int32& Index);

	bool FindItemIndex(FGuid ItemGUID, int32& Index);
		
};
