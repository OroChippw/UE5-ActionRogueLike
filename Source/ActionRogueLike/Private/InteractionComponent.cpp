// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "GamePlayInterface.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractionComponent::PrimaryInteract()
{	
	// 执行碰撞检测和交互
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	 
	// 获取Actor角色的视角信息
	AActor* MyOwner = GetOwner();

	// 设置碰撞检测位置参数，从眼部开始
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation , EyeRotation);

	// // 当然也可以获取玩家（也就是照相机）的视角信息
	// APlayerController* MyController = UGameplayStatics::GetPlayerController(this , 0);
	// MyController->GetPlayerViewPoint(EyeLocation , EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	// // 使用射线单线追踪，检测World Dynamic类型的对象类型之间的碰撞
	// FHitResult HitResult;
	// bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(HitResult , EyeLocation , End , \
	// 				ObjectQueryParams);

	// 使用球形追踪碰撞
	TArray<FHitResult> HitResults; // 检测碰撞的数组

	float Radius = 30.0f;

	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(HitResults , EyeLocation , End , \
										FQuat::Identity , ObjectQueryParams , Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (FHitResult &HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UGamePlayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				IGamePlayInterface::Execute_Interact(HitActor , MyPawn);
				break; // 加个break防止同时打开数个交互对象
			}
		}

		DrawDebugSphere(GetWorld() , HitResult.ImpactPoint , Radius , 32 , LineColor , false , 2.0f);
	}
	

	DrawDebugLine(GetWorld() , EyeLocation , End , LineColor , false, 2.0f , 0 , 2.0f);
}