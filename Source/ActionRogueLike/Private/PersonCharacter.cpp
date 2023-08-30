// Fill out your copyright notice in the Description page of Project Settings.

#include "PersonCharacter.h"
#include "InteractionComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



// Sets default values
APersonCharacter::APersonCharacter()
{
 	// 将此角色设置为每帧调用Tick()。  如果不需要此特性，可以关闭以提升性能。
	PrimaryActorTick.bCanEverTick = true;

	// 赋值的名字"SpringArmComp"将会直接在编辑器里面显示
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	 // 控制角色旋转是否跟随控制器旋转的属性
	SpringArmComp->bUsePawnControlRotation = true;
	// 确保弹簧臂连接在了角色上面
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	// 添加交互组件
	InteractionComp = CreateDefaultSubobject<UInteractionComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 角色的偏航旋转将不受控制器输入的影响
	bUseControllerRotationYaw = false;

	// 初始化部分属性值

}

// Called when the game starts or when spawned
void APersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

void APersonCharacter::MoveForward(float Value)
{
	// 想要人物跟着相机的方向进行移动，而不是角色的朝向移动
	// 保留了控制器在 Yaw 轴上的旋转
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

void APersonCharacter::MoveJump()
{
	bool canJump = true;
	if (canJump)
	{
		bPressedJump = true;
		Jump();
	}	
}

void APersonCharacter::StopJump()
{
	bPressedJump = false;
}

void APersonCharacter::ToggleCrouch()
{
	bool canCrouch = true;
	if (canCrouch && !bIsCrouched)
	{
		Crouch();
		bIsCrouched = true;
	}
}

void APersonCharacter::StopCrouch()
{
	if (bIsCrouched)
	{
		UnCrouch();
		bIsCrouched = false;
	}
}

void APersonCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim); // 添加攻击抬手动画

	// 添加计时器产生延迟避免在抬手前就已经释放魔法弹，0.2秒后才触发
	// 方法二是使用动画事件 TODO
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack , \
			this , &APersonCharacter::PrimaryAttack_TimeElapsed , 0.2f);
	
	// 清楚掉掉计时器
	// GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}

void APersonCharacter::PrimaryAttack_TimeElapsed()
{
	FVector RightHandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FTransform SpawnToMatrix = FTransform(GetControlRotation() , RightHandLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ProjectileClass , SpawnToMatrix , SpawnParams);
}

void APersonCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}


// Called every frame
void APersonCharacter::Tick(float DeltaTime)
{
	// // 绘制一个角色和控制器的方向调试箭头
	// FVector LineStart = GetActorLocation(); // UE中FVector是左手坐标系
	// // FVector DrawScale(1.0f, 1.0f, 1.0f); // 默认大小
	// float DrawScale = 1.0f; // 默认大小

	// float Thickness = 1.0f; // 默认线条粗细
	
	// LineStart += GetActorRightVector() * 100.0f;// 将起点位置向右侧偏移
	// // 根据角色的前方方向设置射线终点
	// FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// // 绘制角色前方方向的调试箭头
	// DrawDebugDirectionalArrow(
	// 	GetWorld() , LineStart , ActorDirection_LineEnd , DrawScale , \
	// 	FColor::Yellow , false , 0.0f , 0 , Thickness
	// );

	// // 根据控制器的旋转设置射线终点
	// FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// // 绘制控制器旋转的调试箭头
	// DrawDebugDirectionalArrow(
    // 	GetWorld() , LineStart , ControllerDirection_LineEnd , DrawScale , \
    // 	FColor::Green , false , 0.0f , 0 , Thickness
	// );

	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 绑定轴实现键盘WASD键前后左右移动
	PlayerInputComponent->BindAxis("MoveForward" , this , &APersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight" , this , &APersonCharacter::MoveRight);
	

	// 绑定Yaw实现左右水平偏航，Pitch是俯仰倾斜旋转，Roll是侧翻滚动
	PlayerInputComponent->BindAxis("Turn" , this , &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp" , this , &APawn::AddControllerPitchInput);

	// 绑定动作实现攻击发射魔法弹
	PlayerInputComponent->BindAction("PrimaryAttack" , IE_Pressed , this , &APersonCharacter::PrimaryAttack);

	// 绑定空格键实现角色的跳跃、shift键实现角色的下蹲
	PlayerInputComponent->BindAction("MoveJump" , IE_Pressed , this , &APersonCharacter::MoveJump);
	PlayerInputComponent->BindAction("MoveJump" , IE_Released , this , &APersonCharacter::StopJump);
	PlayerInputComponent->BindAction("Crouch" , IE_Pressed , this , &APersonCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("Crouch" , IE_Released , this , &APersonCharacter::StopCrouch);
	
	// 绑定E键位实现与其他Actor的交互
	PlayerInputComponent->BindAction("PrimaryInteract" , IE_Pressed , this , &APersonCharacter::PrimaryInteract);
}

