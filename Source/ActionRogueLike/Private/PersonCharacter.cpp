// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "PersonCharacter.h"

// Sets default values
APersonCharacter::APersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 赋值的名字"SpringArmComp"将会直接在编辑器里面显示
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	// 确保弹簧臂连接在了角色上面
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void APersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APersonCharacter::MoveForward(float Value)
{
	// 传递一个想要移动的方向
	AddMovementInput(GetActorForwardVector() , Value);
}

// Called every frame
void APersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 绑定轴实现摁键盘WS键前后移动
	PlayerInputComponent->BindAxis("MoveForward" , this , &APersonCharacter::MoveForward);

	// 绑定欧拉角中的Yaw实现水平偏航，Pitch是垂直旋转，Roll是滚动
	PlayerInputComponent->BindAxis("Turn" , this , &APawn::AddControllerYawInput);

}

