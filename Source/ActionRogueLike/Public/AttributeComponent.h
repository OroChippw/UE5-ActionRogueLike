// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

// 定义一个带有四个参数的动态多播委托，响应UI上的血条变化事件
// 依次传入每个函数参数的类型和名称，AActor* 为第一个参数的数据类型，InstigatorActor为名称，其他同理
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged , AActor* , InstigatorActor , UAttributeComponent* , OwningComp , float , NewHealth , float , Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

protected:
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Attributes")
	float Health; // 血量
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Attributes")
	float HealthMax;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Attributes")
	float Mana; // 法力值
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Attributes")
	float ManaMax;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Attributes")
	float Rage; // 耐力值
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Attributes")
	float RageMax;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Attributes")
	float Strength; // 力量

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// BlueprintAssignable用于声明一个蓝图编程的委托（Delegate）或事件（Event）。
	UPROPERTY(BlueprintAssignable); 
	FOnHealthChanged OnHealthChanged;
	
	UFUNCTION(BlueprintCallable , Category = "Attributes")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable , Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor , float Delta);


	UFUNCTION(BlueprintCallable , Category = "Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable , Category = "Attributes")
	float GetRage() const;



	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
