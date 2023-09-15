// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupActor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
APowerupActor::APowerupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 禁用碰撞，因为碰撞已经由SphereComp进行处理
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;
	bIsActive = true;

}

void APowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	// true代表是否递归设置所有子组件的可见性
	RootComponent->SetVisibility(bIsActive , true);
}

void APowerupActor::SetPowerupState(bool newActiveState)
{
	bIsActive = newActiveState;
	OnRep_IsActive();
}

void APowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

void APowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);
	// 吃掉后隐藏Powerup道具
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer , this , &APowerupActor::ShowPowerup , RespawnTime);
}

void APowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in derived classes...
}

FText APowerupActor::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

// 使用ReplicatedUsing = "OnRep_IsActive"实现完了过同步功能，就要定义相关的同步属性
// 描述该函数返回一个描述哪些属性需要进行网络同步的清单，需要引入Net/UnrealNetwork.h
void APowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// 指示 Unreal Engine 在网络中同步 APowerupActor 类中的 bIsActive 属性。
	// 这意味着当 bIsActive 的值在服务器上更改时，它将自动同步到所有客户端
	DOREPLIFETIME(APowerupActor, bIsActive);
}

// Called when the game starts or when spawned
void APowerupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

