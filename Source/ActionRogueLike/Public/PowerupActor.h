// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GamePlayInterface.h"
#include "PowerupActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API APowerupActor : public AActor , public IGamePlayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerupActor();

protected:
	// ReplicatedUsing是属性标记的一部分，说明该属性需要进行网络复制，
	// 指定OnRep_IsActive函数名，在属性被复制时会调用该函数
	// 这种设置通常用于在属性值在客户端和服务器之间同步时，执行一些特定的逻辑，以确保游戏状态保持一致
	UPROPERTY(ReplicatedUsing = "OnRep_IsActive") 
	bool bIsActive;

	UPROPERTY(EditAnywhere , Category = "Powerup")
	float RespawnTime; // 重置时间
	FTimerHandle TimerHandle_RespawnTimer;

	UPROPERTY(VisibleAnywhere , Category = "Components")
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere , Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UFUNCTION()
	void OnRep_IsActive();

	UFUNCTION()
	void ShowPowerup();

	void HideAndCooldownPowerup();

	void SetPowerupState(bool newActiveState);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
