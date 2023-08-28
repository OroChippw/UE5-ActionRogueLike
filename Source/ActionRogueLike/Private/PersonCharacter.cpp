// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "PersonCharacter.h"

// Sets default values
APersonCharacter::APersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 赋值的名字"SpringArmComp"将会直接在编辑器里面显示
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	 // 控制角色旋转是否跟随控制器旋转的属性
	SpringArmComp->bUsePawnControlRotation = true;
	// 确保弹簧臂连接在了角色上面
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 角色的偏航旋转将不受控制器输入的影响
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void APersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APersonCharacter::MoveForward(float Value)
{
	// 想要人物跟着相机的方向进行移动，而不是角色的朝向移动
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// 传递一个想要移动的方向,Value是移动单位
	AddMovementInput(ControlRot.Vector() , Value);
}

void APersonCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector , Value);
}

void APersonCharacter::PrimaryAttack()
{

	FVector RightHandLocation = GetMesh()->GetSocketLocation("Muzzle_01");


	FTransform SpawnToMatrix = FTransform(GetControlRotation() , RightHandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ProjectileClass , SpawnToMatrix , SpawnParams);

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

	// 绑定轴实现键盘WASD键前后左右移动
	PlayerInputComponent->BindAxis("MoveForward" , this , &APersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight" , this , &APersonCharacter::MoveRight);


	// 绑定欧拉角中的Yaw实现水平偏航，Pitch是垂直旋转，Roll是滚动
	PlayerInputComponent->BindAxis("Turn" , this , &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp" , this , &APawn::AddControllerPitchInput);

	// 绑定动作实现攻击发射魔法弹
	PlayerInputComponent->BindAction("PrimaryAttack" , IE_Pressed , this , &APersonCharacter::PrimaryAttack);
}

