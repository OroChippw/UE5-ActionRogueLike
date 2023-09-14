// Fill out your copyright notice in the Description page of Project Settings.


#include "DashProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"

ADashProjectile::ADashProjectile()
{
    DetonateDelay = 0.2f;
	TeleportDelay = 0.2f;
    MovementComp->InitialSpeed = 6000.0f;
}

void ADashProjectile::TeleportInstigator()
{
    // APawn* Instigator为人物本身，调用actor的TeleportTo函数实现传送
    AActor* TeleportActor = GetInstigator();
    if (ensure(TeleportActor))
    {
        TeleportActor->TeleportTo(GetActorLocation() , TeleportActor->GetActorRotation() , false , false);
    }
}

void ADashProjectile::Explode_Implementation()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);
    
    if (ensure(IsValid(this)))
    {   
        UGameplayStatics::SpawnEmitterAtLocation(this , ImpactVFX , GetActorLocation() , GetActorRotation());
        // 爆炸后取消粒子效果，并停止子弹的移动与碰撞
        EffectComp->DeactivateSystem();
        MovementComp->StopMovementImmediately();
        SetActorEnableCollision(false);

    	FTimerHandle TimerHandle_DelayedTeleport; // 延迟传送句柄
        // 设定计时器进行触发
        GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport , this , &ADashProjectile::TeleportInstigator , TeleportDelay);
    }
}

void ADashProjectile::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate , this , &ADashProjectile::Explode , DetonateDelay);
}

