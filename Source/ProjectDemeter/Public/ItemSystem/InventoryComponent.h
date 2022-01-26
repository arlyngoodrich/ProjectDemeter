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

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	bool RemoveItem(FItemData Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClientFriendly_RemoveItem(FItemData Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	bool ConsumeItem(FItemData Item, AActor* TargetActor, AController* InstigatingPlayer);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void  ClientFriendly_ConsumeItem(FItemData Item, AActor* TargetActor, AController* InstigatingPlayer);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxItems;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_InventoryUpdate, Category = "Inventory")
	TArray<FItemData> Inventory;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveItem(FItemData Item);
	bool Server_RemoveItem_Validate(FItemData Item);
	void Server_RemoveItem_Implementation(FItemData Item);


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ConsumeItem(FItemData Item, AActor* TargetActor, AController* InstigatingPlayer);
	bool Server_ConsumeItem_Validate(FItemData Item, AActor* TargetActor, AController* InstigatingPlayer);
	void Server_ConsumeItem_Implementation(FItemData Item, AActor* TargetActor, AController* InstigatingPlayer);

	UFUNCTION()
	void OnRep_InventoryUpdate();

private:

	bool FindFirstIndexOfItem(FItemData Item, int32& Index);

		
};
