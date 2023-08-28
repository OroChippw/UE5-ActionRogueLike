// Fill out your copyright notice in the Description page of Project Settings.
#include "MagicActor.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AMagicActor::AMagicActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	// SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	// // 先将所有的通道设置为ignore，再单独设置Pawn类型的Actor做Overlap响应
	// SphereComp->SetCollisionResponseToChannel(ECC_Pawn , ECR_Overlap);
	// SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	// 如果已经在项目里面Preset了Collision，则可以直接调用自定义配置不需要像上述一样每个通道逐个定义
	SphereComp->SetCollisionProfileName("Projectile");

	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f; // 设置抛射物的初始速度
	MovementComp->bRotationFollowsVelocity = true; // 设置抛射物的旋转会随着其移动的方向而改变
	/*
		bInitialVelocityInLocalSpace 属性用于决定抛射物的初始速度是在局部空间中还是世界空间中指定的。
	这个属性可以影响抛射物的初始速度方向和行为。
	*/ 
	MovementComp->bInitialVelocityInLocalSpace = true;


}

// Called when the game starts or when spawned
void AMagicActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMagicActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

