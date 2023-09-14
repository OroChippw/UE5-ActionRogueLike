// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
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
	SphereComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnActorHit);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f; // 设置抛射物的初始速度
	MovementComp->bRotationFollowsVelocity = true; // 设置抛射物的旋转会随着其移动的方向而改变
	// 设置发射物的初始速度基于自身的坐标系
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f; // 将发射物的重力缩放系数设置为零,保持直线飞行

	
	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);

	// 在需要的时候调用 RadialForceComponent->FireImpulse() 来触发力效果
	ExplosionForceComp = CreateDefaultSubobject<URadialForceComponent>("ExplosionForceComp");
	ExplosionForceComp->SetupAttachment(RootComponent);

	ImpactShakeInnerRadius = 0.0f;
	ImpactShakeOuterRadius = 1500.0f;
}

void AProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

// 在一个 C++ 类中声明了一个 Blueprint 本地事件,需要为该事件提供一个 C++ 基本实现
// 为了区分基本实现和蓝图实现，Unreal Engine 的规范是使用 _Implementation 后缀来标记 C++ 基本实现
void AProjectileBase::Explode_Implementation()
{
	if (ensure(IsValid(this)))
	{
		// 在指定位置生成粒子效果
		UGameplayStatics::SpawnEmitterAtLocation(this , ImpactVFX  , GetActorLocation() , GetActorRotation());

		// 在指定位置播放声音效果
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());

		// 触发摄像机的震动效果
		UGameplayStatics::PlayWorldCameraShake(this , ImpactShake , GetActorLocation() , ImpactShakeInnerRadius , ImpactShakeOuterRadius);

		// 爆炸执行完毕，将该游戏对象进行消除
		Destroy();

	}
}

void AProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

