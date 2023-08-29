// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GamePlayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGamePlayInterface : public UInterface
{
	// 实现Interface的目的是实现某种共享属性的通用api，而不需要特殊指定
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONROGUELIKE_API IGamePlayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 实现交互功能，并接入各个Actor里面，并使用UFUNC将其暴露在蓝图里面
	UFUNCTION(BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
};
