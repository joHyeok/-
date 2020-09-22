// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterItem.h"
#include "ItemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StreamableManager.h"
#include "ItemDataTable.h"
#include "Net/UnrealNetwork.h"
#include "../Basic/BasicPlayer.h"

// Sets default values
AMasterItem::AMasterItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("Item"));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->SetSphereRadius(150.0f);

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
		//호스트에서만 실행되도록 한다.
		//클라이언트가 아이템을 선택하지 않고 서버만 선택해 나머지는 동기화하도록 한다.
		if (HasAuthority())
		{
			//아이템 랜덤 설정
			ItemIndex = FMath::RandRange(1, 6) * 10;
		}
		//선택된 설정을 서버와 클라이언트 둘다 적용
		ItemData = ItemComponent->GetItemData(ItemIndex);
		if (ItemIndex != 0)
		{
			//
			FStreamableManager Loader;
			StaticMesh->SetStaticMesh(Loader.LoadSynchronous<UStaticMesh>(ItemData.ItemMesh));
		}
	}

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMasterItem::ProcessBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AMasterItem::ProcessEndOverlap);
}

// Called every frame
void AMasterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMasterItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMasterItem, ItemIndex);
}

void AMasterItem::ProcessBeginOverlap(UPrimitiveComponent * OverlappedComponent, 
	AActor * OtherActor,
	UPrimitiveComponent * OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		ABasicPlayer* Pawn = Cast<ABasicPlayer>(OtherActor);

		//콜리전에 겹쳤다는 건 다른 서버나 클라이언트들도 겹쳤다는 뜻이 된다.
		//UI가 출력되진 않지만 아래 코드를 호출은 하게 된다.
		//IsLocallyControlled()로 로컬인지 검사하자
		if (Pawn && Pawn->IsLocallyControlled())
		{
			//UE_LOG(LogClass, Warning, TEXT("ProcessBeginOverlap"));
			//자기자신을 추가
			Pawn->AddPickItem(this);
		}
	}
}

void AMasterItem::ProcessEndOverlap(UPrimitiveComponent * OverlappedComponent,
	AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		ABasicPlayer* Pawn = Cast<ABasicPlayer>(OtherActor);
		if (Pawn && Pawn->IsLocallyControlled())
		{
			//UE_LOG(LogClass, Warning, TEXT("ProcessEndOverlap"));
			//자기자신을 제거
			Pawn->RemovePickItem(this);
		}
	}
}

