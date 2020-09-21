// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterItem.h"
#include "ItemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StreamableManager.h"
#include "ItemDataTable.h"

// Sets default values
AMasterItem::AMasterItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("Item"));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	//네트워크상 동기화하기
	bReplicates = true;
}

// Called when the game starts or when spawned
void AMasterItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (ItemComponent->ItemDataTable)
	{
		//아이템 랜덤 설정
		ItemIndex = FMath::RandRange(1, 6) * 10;
		ItemData = ItemComponent->GetItemData(ItemIndex);
		if (ItemIndex != 0)
		{
			//
			FStreamableManager Loader;
			StaticMesh->SetStaticMesh(Loader.LoadSynchronous<UStaticMesh>(ItemData.ItemMesh));
		}
	}
}

// Called every frame
void AMasterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

