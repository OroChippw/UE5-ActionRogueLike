// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PersonCharacter.generated.h"

// 进行简单的定义告诉编译器我们在处理一个类
class UCameraComponent;
class USpringArmComponent;
class UInteractionComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API APersonCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	bool test;
	float AttackAnimDelay = 0.2f;

protected: 
	UPROPERTY(VisibleAnywhere) // 允许UE编辑器能看到所有的变量，但不能编辑
	USpringArmComponent* SpringArmComp; // 创建摄像机视角跟随游戏对象

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp; // 创建摄像机
	
	UPROPERTY(VisibleAnywhere)
	UInteractionComponent* InteractionComp; // 创建和管理游戏中的交互性和交互行为

	// EditAnywhere允许UE编辑器中编辑该属性的值，无论是实例级别还是类级别	
	UPROPERTY(EditAnywhere , Category = "Attack") // Category则是显示属性的哪个类别
	TSubclassOf<AActor> ProjectileClass; // 定义各种子类

	UPROPERTY(EditAnywhere , Category = "Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;
	 
	UPROPERTY(EditAnywhere , Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass; 

	UPROPERTY(EditAnywhere , Category = "Attack") 
	UAnimMontage* AttackAnim; // 定义攻击动画

	UPROPERTY(EditAnywhere , Category = "Attack")
	UParticleSystem* CastingEffect; // 定义特效粒子

	// 定义攻击定时器句柄
	FTimerHandle TimerHandle_PrimaryAttack; 
	FTimerHandle TimerHandle_BlackHoleAttack;
	FTimerHandle TimerHandle_Dash;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Character Action
	void MoveForward(float value);
	void MoveRight(float value);
	
	void MoveJump();
	void StopJump();

	void ToggleCrouch();
	void StopCrouch();

	// Attack Action
	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn , FName SocketName);
	void AttackEffects();
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();
	void BlackHoleAttack();
	void BlackHoleAttack_TimeElapsed();
	void Dash();
	void Dash_TimeElapsed();


	// Interaction Action
	void PrimaryInteract();

public:
	// Sets default values for this character's properties
	APersonCharacter();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
