// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{

	HealthMax = 100.0f;
	Health = HealthMax;

	ManaMax = 200.0f;
	Mana = ManaMax;

	RageMax = 100.0f;
	Rage = 0;

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

float UAttributeComponent::GetHealth() const
{
	return Health;
}

float UAttributeComponent::GetMana() const
{
	return Mana;
}

float UAttributeComponent::GetRage() const
{
	return Rage;
}

bool UAttributeComponent::ApplyHealthChange(AActor* InstigatorActor , float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr , this , Health , Delta);

	return true;
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

