// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSystem/InventoryComponent.h"
#include "Core/Logs_C.h"
#include "AttributeSystem/StatEffect.h"
#include "ItemSystem/InventoryWidget.h"

//UE4 Includes
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	MaxItems = 5;


	// ...
}

int32 UInventoryComponent::GetMaxInventorySlots() const {return MaxItems;}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	SetOwningPlayer();

	if(bIsOwnedByPlayer)
	{
		CreateInventoryWidget(OwningPlayer);
	}
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Inventory);

}

void UInventoryComponent::OnRep_InventoryUpdate() const
{
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::CreateInventoryWidget(APlayerController* PlayerController)
{
	if(InventoryWidgetClass == nullptr)
	{
		UE_LOG(LogInventorySystem,Error,TEXT("%s class inventory widget class is null"),*GetClass()->GetName());
		return;
	}

	if(PlayerController == nullptr)
	{
		UE_LOG(LogInventorySystem,Error,TEXT("%s attempting to create class without valid player controller"))
		return;
	}

	//Only create widget if controller is local 
	if(PlayerController->IsLocalController())
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(PlayerController,InventoryWidgetClass);
		InventoryWidget->SetUpInventoryWidget(this);
		InventoryWidget->AddToViewport();
	}
}

void UInventoryComponent::ClientFriendly_RemoveItem(FItemData Item)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		Server_RemoveItem(Item);
	}
	else
	{
		RemoveItem(Item);
	}
}


void UInventoryComponent::ClientFriendly_ConsumeItem(FItemData Item, AActor* TargetActor)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		ConsumeItem(Item, TargetActor);
	}
	else
	{
		Server_ConsumeItem(Item, TargetActor);
	}
}

bool UInventoryComponent::AddItem(const FItemData Item)
{

	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogInventorySystem, Log, TEXT("Attempting to add %s item to %s inventory as non-authority"), *Item.DisplayName.ToString(), *GetOwner()->GetName())
			
		return false;
	}



	if (Inventory.Num() == MaxItems)
	{
		UE_LOG(LogInventorySystem,Log,TEXT("Attempting to add %s item but inventory for %s is full"), *Item.DisplayName.ToString(), *GetOwner()->GetName())

		return false; 
	}

	Inventory.Add(Item);
	UE_LOG(LogInventorySystem, Log, TEXT("%s was added to %s's inventory"), *Item.DisplayName.ToString(), *GetOwner()->GetName());
	
	OnRep_InventoryUpdate();
	
	return true;

}

bool UInventoryComponent::RemoveItem(const FItemData Item)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogInventorySystem, Log, TEXT("Attempting to remove %s item from %s inventory as non-authority"), *Item.DisplayName.ToString(), *GetOwner()->GetName())

			return false;
	}

	int32 ItemIndex;
	if (FindFirstIndexOfItem(Item,ItemIndex))
	{
		Inventory.RemoveAt(ItemIndex);
		OnRep_InventoryUpdate();

		UE_LOG(LogInventorySystem, Log, TEXT("%s removed from %s inventory."), *Item.DisplayName.ToString(), *GetOwner()->GetName())
		return true;
	}
	else
	{

		UE_LOG(LogInventorySystem,Log,TEXT("%s not found in %s inventory.  Could not remove item"),*Item.DisplayName.ToString(),*GetOwner()->GetName())
		return false;
	}
}

bool UInventoryComponent::ConsumeItem(FItemData Item, AActor* TargetActor)
{
	//Confirm Authority
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogInventorySystem, Log, TEXT("Attempting to consume %s item in %s inventory as non-authority"), *Item.DisplayName.ToString(), *GetOwner()->GetName());
		return false;
	}

	//Confirm in inventory
	int32 ItemIndex;
	if (FindFirstIndexOfItem(Item, ItemIndex) == false)
	{
		UE_LOG(LogInventorySystem,Error,TEXT("Attempted to consume %s item from %s inventory when item not server inventory "),*Item.DisplayName.ToString(),*GetOwner()->GetName())
		return false;
	}

	//Confirm target actor and player controller are not null 
	if (TargetActor == nullptr)
	{
		UE_LOG(LogInventorySystem,Error,TEXT("Attempted to consume %s item from %s inventory Target Actor is null"), *Item.DisplayName.ToString(), *GetOwner()->GetName())
		return false;
	}

	UStatEffect* CreatedEffect = NewObject<UStatEffect>(this, Item.StatEffectOnConsume);

	//Initialize Effect
	CreatedEffect->InitalizeEffect(TargetActor);

	//Trigger Effect
	if (CreatedEffect->bReadyToTriggerEffect)
	{
		if (CreatedEffect->TriggerEffect())
		{
			if (RemoveItem(Item))
			{
				return true;
			}
		}
	}

	return false;
}


void UInventoryComponent::SetOwningPlayer()
{
	ACharacter* OwningCharacter = Cast<ACharacter>(GetOwner());
	if(OwningCharacter)
	{
		APlayerController* PlayerController = Cast<APlayerController>(OwningCharacter->GetController());
		if(PlayerController)
		{
			bIsOwnedByPlayer = true;
			OwningPlayer = PlayerController;
		}
	}
}


bool UInventoryComponent::FindFirstIndexOfItem(FItemData Item, int32& Index)
{

	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i].DisplayName == Item.DisplayName)
		{
			Index = i;
			return true;
		}
	}

	return false;
}


bool UInventoryComponent::Server_RemoveItem_Validate(FItemData Item)
{
	return true;
}

void UInventoryComponent::Server_RemoveItem_Implementation(FItemData Item)
{
	RemoveItem(Item);
}

bool UInventoryComponent::Server_ConsumeItem_Validate(FItemData Item, AActor* TargetActor)
{
	return true;
}

void UInventoryComponent::Server_ConsumeItem_Implementation(FItemData Item, AActor* TargetActor)
{
	ConsumeItem(Item, TargetActor);
}