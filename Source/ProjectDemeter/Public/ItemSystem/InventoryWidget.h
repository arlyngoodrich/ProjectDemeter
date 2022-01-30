// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;

/**
 * 
 */
UCLASS()
class PROJECTDEMETER_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	//Owning inventory set during initialization called by creating inventory 
	UPROPERTY(BlueprintReadOnly, Category="Inventory System")
	UInventoryComponent* OwningInventory;

	//Max number of inventory slots in owning inventory.  Set during initialization
	UPROPERTY(BlueprintReadOnly, Category="Inventory System")
	int32 NumberOfSlots;

	//Called by creating inventory component
	void SetUpInventoryWidget(UInventoryComponent* CreatingInventory);

protected:

	UFUNCTION(BlueprintCallable, Category="Inventory System")
	void ConsumeItem(FItemData ItemToConsume);
	
	//Called by native class when inventory is updated.  Utilizes Inventory Component delegate.  
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory System", DisplayName = "Refresh Inventory UI")
	void BP_RefreshInventoryUI();

	//Internal overridable function that is bound to owning inventory component 'On Update' delegate
	UFUNCTION()
	virtual void Internal_RefreshInventory();

	virtual void NativeConstruct() override;
	
};
