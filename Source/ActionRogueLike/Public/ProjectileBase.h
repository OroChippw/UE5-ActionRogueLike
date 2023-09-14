// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
// class UNiagaraComponent;
class URadialForceComponent;

// class UParticleSystem;
class UCameraShakeBase;
class UAudioComponent;
class USoundCue;

UCLASS(ABSTRACT) // 'ABSTRACT'表示这个类还没完成，不会出现在UE编辑器的下拉窗口中
class ACTIONROGUELIKE_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:

	// 定义球体作为碰撞组件
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = "Components")
	USphereComponent* SphereComp;

	// 抛射物组件，用以初始速度/方向，遵循不同的弹道轨迹
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = "Components")
	UProjectileMovementComponent* MovementComp;

	// 管理和渲染粒子系统的组件，控制粒子的外观、行为、材质
	// UParticleSystemComponent基于Cascade的旧版粒子系统
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = "Components")
	UParticleSystemComponent* EffectComp;
	// UNiagaraComponent 基于Niagara的新一代粒子系统，具有更高的可编程性和灵活性
	// UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = "Components")
	// UNiagaraComponent* NiagaraLoopComp;

		/* ****** Components ****** */

	// 管理音频系统的组件，控制发射物的音效
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = "Components")
	UAudioComponent* AudioComp;

	// 一定范围内施加径向力的组件，实现爆炸效果、冲击波效果以及其他需要在一定范围内推动或拉近物体的场景
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = "Components")
	URadialForceComponent* ExplosionForceComp;

	/* ****** Effects ****** */

	// 实现粒子击中震动效果
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	TSubclassOf<UCameraShakeBase> ImpactShake;

	// 摄像机震动效果的内部半径，当游戏对象在内部半径内时，摄像机震动效果的强度可能更大
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeInnerRadius;

	// 摄像机震动效果的外部半径，当游戏对象位于内部半径和外部半径之间时，摄像机震动效果的强度逐渐减小。
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeOuterRadius;

	// 子弹爆炸音效,USoundCue是声音效果的定义和配置，UAudioComponent则是游戏中实际播放和控制声音的组件
	UPROPERTY(EditDefaultsOnly , Category = "Effects")
	USoundCue* ImpactSound;

	// 实现粒子爆炸效果组件
	UPROPERTY(EditDefaultsOnly , Category = "Effects")
	UParticleSystem* ImpactVFX;

	// 处理游戏对象与其他物体发生碰撞时的事件
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , FVector NormalImpluse , const FHitResult& Hit);
	
	// 允许子类在蓝图中定义和触发爆炸效果
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
