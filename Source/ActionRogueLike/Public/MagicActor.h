// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicActor.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API AMagicActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagicActor();

protected:
	// 定义球体作为碰撞组件
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;

	// 抛射物组件，用以初始速度/方向，遵循不同的弹道轨迹
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MovementComp;

	// 管理和渲染粒子系统的组件，控制粒子的外观、行为、材质
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* EffectComp;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
