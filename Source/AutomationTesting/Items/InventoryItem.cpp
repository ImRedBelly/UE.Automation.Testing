// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItem.h"

#include "Components/InventoryComponent.h"
#include "Components/SphereComponent.h"

AInventoryItem::AInventoryItem()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	check(CollisionComponent);
	CollisionComponent->InitSphereRadius(30.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);
	SetRootComponent(CollisionComponent);
}

void AInventoryItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (const auto Pawn = Cast<APawn>(OtherActor))
	{
		if (const auto InventoryComponent = Pawn->FindComponentByClass<UInventoryComponent>())
		{
			if (InventoryComponent->TryToAddItem(InventoryData))
			{
				Destroy();
			}
		}
	}
}
