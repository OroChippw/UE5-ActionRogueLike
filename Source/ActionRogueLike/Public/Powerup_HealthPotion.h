// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerupActor.h"
#include "Powerup_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API APowerup_HealthPotion : public APowerupActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APowerup_HealthPotion();

protected:
	UPROPERTY(EditAnywhere , Category = "HealthPotion")
	int32 CreditCost; // 治疗量

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;


};
