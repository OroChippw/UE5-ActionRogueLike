// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

// 定义一个带有四个参数的动态多播委托，响应UI上的血条变化事件
// 依次传入每个函数参数的类型和名称，AActor* 为第一个参数的数据类型，InstigatorActor为名称，其他同理
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged , AActor* , InstigatorActor , UAttributeComponent* , OwningComp , float , NewValue , float , Delta);

// BlueprintSpawnableComponent标识可以在蓝图中将这个组件直接拖放到 Actor 上，而不需要通过 C++ 代码手动创建和附加组件
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable , Category = "Attributes")
	static UAttributeComponent* GetAttribute(AActor* FromActor); 

	// meta用以指定在蓝图编辑器里面显示函数的名称
	UFUNCTION(BlueprintCallable , Category = "Attributes" , meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

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

	// NetMulticast标记一个多播函数，用于服务器和客户端之间的同步调用
	// Reliable表示该多播可靠，会尽力确保消息的可靠传递，防止消息的丢失
	UFUNCTION(NetMulticast , Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor , float NewHealth , float Delta);

	UFUNCTION(NetMulticast , Unreliable)
	void MulticastManaChanged(AActor* InstigatorActor , float NewMana , float Delta);

	UFUNCTION(NetMulticast , Unreliable)
	void MulticastRageChanged(AActor* InstigatorActor , float NewRage , float Delta);


	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	/* ****** State ****** */
	UFUNCTION(BlueprintCallable , Category = "Attributes")
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable , Category = "Attributes")
	bool isAlive() const;

	/* ****** Health ****** */
	UFUNCTION(BlueprintCallable , Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable , Category = "Attributes")
	float GetHealthMax() const;

	UFUNCTION(BlueprintCallable , Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor , float Delta);

	UFUNCTION(BlueprintCallable , Category = "Attributes")
	bool isFullHealth() const;

	/* ****** Mana ****** */
	UFUNCTION(BlueprintCallable , Category = "Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable , Category = "Attributes")
	float GetManaMax() const;

	UFUNCTION(BlueprintCallable , Category = "Attributes")
	bool ApplyManaChange(AActor* InstigatorActor , float Delta);

	/* ****** Rage ****** */
	UFUNCTION(BlueprintCallable , Category = "Attributes")
	float GetRage() const;

	UFUNCTION(BlueprintCallable , Category = "Attributes")
	float GetRageMax() const;

	UFUNCTION(BlueprintCallable , Category = "Attributes")
	bool ApplyRageChange(AActor* InstigatorActor , float Delta);

	/* ****** Multicast Event ****** */
	// BlueprintAssignable用于声明一个蓝图编程的委托（Delegate）或事件（Event）。
	UPROPERTY(BlueprintAssignable , Category = "Attributes"); 
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable , Category = "Attributes"); 
	FOnAttributeChanged OnManaChanged;

	UPROPERTY(BlueprintAssignable , Category = "Attributes"); 
	FOnAttributeChanged OnRageChanged;

	// 添加 GetLifetimeReplicatedProps 函数声明
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
