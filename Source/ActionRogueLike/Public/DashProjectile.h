// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "DashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ADashProjectile : public AProjectileBase
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly , Category = "Teleport")
	float DetonateDelay = 0.2f; // 起爆延迟

	UPROPERTY(EditDefaultsOnly , Category = "Teleport")
	float TeleportDelay = 0.2f; // 传送延迟

	// 定义延迟爆炸句柄
	FTimerHandle TimerHandle_DelayedDetonate; // 如果我们击中了物体,我们需要通过Handle来取消定时爆炸的计时器

	UFUNCTION()
	void TeleportInstigator();

	virtual void Explode_Implementation() override;

	virtual void BeginPlay() override;

public:
	ADashProjectile();
	
};
