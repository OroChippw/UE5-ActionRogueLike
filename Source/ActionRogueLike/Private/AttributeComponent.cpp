// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	HealthMax = 100.0f;
	Health = HealthMax;

	ManaMax = 200.0f;
	Mana = ManaMax;

	RageMax = 100.0f;
	Rage = 0;

	// 设置对象是否应该进行网络复制
	// 在多人游戏中，网络复制是一种机制，用以在不同的客户端之间同步对象的状态
	SetIsReplicatedByDefault(true);
}

bool UAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor , -GetHealthMax());
}

bool UAttributeComponent::isAlive() const
{
	return (Health > 0.0f);
}

float UAttributeComponent::GetHealth() const
{
	return Health;
}

float UAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool UAttributeComponent::isFullHealth() const
{
	return (Health == HealthMax);
}

float UAttributeComponent::GetMana() const
{
	return Mana;
}

float UAttributeComponent::GetManaMax() const
{
	return ManaMax;
}

float UAttributeComponent::GetRage() const
{
	return Rage;
}

float UAttributeComponent::GetRageMax() const
{
	return RageMax;
}

bool UAttributeComponent::ApplyHealthChange(AActor* InstigatorActor , float Delta)
{
	// if (Delta < 0.0f)
	// {
	// 	return false;
	// }
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr , this , Health , Delta);

	return true;
}

bool UAttributeComponent::ApplyManaChange(AActor* InstigatorActor , float Delta)
{
	Mana += Delta;

	OnManaChanged.Broadcast(nullptr , this , Mana , Delta);

	return true;
}

bool UAttributeComponent::ApplyRageChange(AActor* InstigatorActor , float Delta)
{
	Rage += Delta;

	OnHealthChanged.Broadcast(nullptr , this , Rage , Delta);

	return true;
}

void UAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor , float NewHealth , float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor , this , NewHealth , Delta);
}

void UAttributeComponent::MulticastManaChanged_Implementation(AActor* InstigatorActor , float NewMana , float Delta)
{
	OnManaChanged.Broadcast(InstigatorActor , this , NewMana , Delta);
}

void UAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor , float NewRage , float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor , this , NewRage , Delta);
}

UAttributeComponent* UAttributeComponent::GetAttribute(AActor* FromActor)
{
	if (FromActor)
	{	
		return FromActor->FindComponentByClass<UAttributeComponent>();
	}
	return nullptr;
}

bool UAttributeComponent::IsActorAlive(AActor* Actor)
{
	UAttributeComponent* AttributeComp = GetAttribute(Actor);
	if (AttributeComp)
	{
		return AttributeComp->isAlive();
	}
	return false;
}

void UAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAttributeComponent , Health);
	DOREPLIFETIME(UAttributeComponent , HealthMax);

	DOREPLIFETIME(UAttributeComponent , Mana);
	DOREPLIFETIME(UAttributeComponent , ManaMax);

	DOREPLIFETIME(UAttributeComponent , Rage);
	DOREPLIFETIME(UAttributeComponent , RageMax);

}

// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

