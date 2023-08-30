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

protected: 
	UPROPERTY(VisibleAnywhere) // 让编辑器能看到所有的变量
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;
	
	UPROPERTY(VisibleAnywhere)
	UInteractionComponent* InteractionComp;

	UPROPERTY(EditAnywhere , Category = "Attack")
	TSubclassOf<AActor> ProjectileClass; // 定义魔法弹子类

	UPROPERTY(EditAnywhere , Category = "Attack")
	UAnimMontage* AttackAnim; // 定义攻击动画

	FTimerHandle TimerHandle_PrimaryAttack; // 定义攻击定时器句柄

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
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();

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
