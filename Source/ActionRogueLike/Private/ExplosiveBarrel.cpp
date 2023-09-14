// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	// 启用静态网格模拟物理效果
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);

	ForceComp->SetAutoActivate(false);

	// 定义爆炸半径及强度
	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 2500.0f;
	ForceComp->bImpulseVelChange = true; // 忽略其他物体的质量

	// 定义影响的Object type
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);

	// MeshComp->OnComponentHit.AddDynamic(this , &AExplosiveBarrel::OnActorHit)

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AExplosiveBarrel::PostInitializeComponents()
{
	// 确保在执行自定义初始化代码之前执行了基类的初始化操作
	Super::PostInitializeComponents();
	// 当 MeshComp 组件被击中（碰撞）时，将调用 OnActorHit 函数，执行特定的操作
	MeshComp->OnComponentHit.AddDynamic(this , &AExplosiveBarrel::OnActorHit);
}

void AExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , FVector NormalImpulse , const FHitResult& Hit)
{
	ForceComp->FireImpulse(); // 施加一个瞬时力

	UE_LOG(LogTemp , Log , TEXT("OnActorHit in Explosive Barrel"));

	// %s = string , %f = float
	// 输出此形式的日志 logs:"OtherActor:MyActor_1 , at gametime : xxxx.xx"
	UE_LOG(LogTemp , Warning , TEXT("OtherActor : %s , at game time : %f") , *GetNameSafe(OtherActor) , GetWorld()->TimeSeconds);

	FString CombineString = FString::Printf(TEXT("Hit at location : %s") , *Hit.ImpactPoint.ToString());

	DrawDebugString(GetWorld() , Hit.ImpactPoint , CombineString , nullptr , FColor::Green  , 2.0f , true);
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

