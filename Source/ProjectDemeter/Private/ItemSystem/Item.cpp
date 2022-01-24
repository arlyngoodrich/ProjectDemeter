// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSystem/Item.h"
#include "InteractionSystem/InteractableObjectComponent.h"
#include "Core/Logs_C.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteractionComponent = CreateDefaultSubobject<UInteractableObjectComponent>(TEXT("Interaction Component"));
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	Initalize();
	
}

void AItem::Initalize()
{
	InteractionComponent->OnInteractionTriggered.AddDynamic(this, &AItem::OnInteraction);
}

void AItem::OnInteraction(AActor* InstigatingActor)
{
	UE_LOG(LogInteractionSystem,Log,TEXT("%s received interaction from %s"),*GetName(),*InstigatingActor->GetName())
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

