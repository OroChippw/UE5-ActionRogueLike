// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GamePlayInterface.h"
#include "ItemChest.generated.h"


class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API AItemChest : public AActor , public IGamePlayInterface 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float TargetPitch = 0.0f; // 箱盖和主题的俯仰开合程度
	
	void Interact_Implementation(APawn* InstigatorPawn);

public:	
	// Sets default values for this actor's properties
	AItemChest();

protected:
	// Chest的主体Mesh
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	// Chest的盖子Mesh
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
