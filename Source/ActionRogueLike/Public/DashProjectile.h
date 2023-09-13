// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagicActor.h"
#include "DashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ADashProjectile : public AMagicActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ADashProjectile();
	
};
