// Fill out your copyright notice in the Description page of Project Settings.

#include "PersonCharacter.h"
#include "InteractionComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
APersonCharacter::APersonCharacter()
{
 	// 将此角色设置为每帧调用Tick(),如果不需要此特性，可以关闭以提升性能。
	PrimaryActorTick.bCanEverTick = true;

	// 赋值的名字"SpringArmComp"将会直接在编辑器里面显示
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	// 控制角色旋转是否跟随控制器旋转的属性
	SpringArmComp->bUsePawnControlRotation = true;
	// 弹簧臂附着在RootComponent，摄像机附着在弹簧臂上
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	// 添加交互组件
	InteractionComp = CreateDefaultSubobject<UInteractionComponent>("InteractionComp");

	// 添加自定义属性组件
	AttributeComp = CreateDefaultSubobject<UAttributeComponent>("AttributeComp");

	// 控制角色朝移动方向自动旋转
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 保证角色的偏航旋转将不受控制器输入的影响
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
	
	// 获取玩家控制器创建旋转矩阵，再获取Y轴的单位向量
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector , Value);
}

void APersonCharacter::MoveJump()
{
	bool canJump = true;
	if (canJump) // 加个是否可以跳跃的状态以后可以添加判断是否有阻挡物影响跳跃
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
	// 蹲伏
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

void APersonCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn , FName SocketName)
{
	// 使用ensure实现assert的断言效果，编译后只触发一次
	// 持续触发使用ensureAlways，在进行游戏打包时要去除断言
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation(SocketName);

		// 生成Actor的参数对象，生成的Actor不管是否存在碰撞都应该生成
		FActorSpawnParameters SpawnParams; 
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this; // 设置生成的Actor的发起者为当前角色

		// 创建碰撞参数对象，并将当前角色添加进忽略列表
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		// 创建碰撞对象为一个半径20.0f的球体
		FCollisionShape CollisionShape;
		CollisionShape.SetSphere(20.0f);

		// 创建碰撞对象查询参数对象，并添加查询列表
		FCollisionObjectQueryParams CollsionQueryParams;
		CollsionQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		CollsionQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		CollsionQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		// 获取摄像机组件的位置作为碰撞查询的起始点，假设相机前方5000个单位为碰撞查询终点
		FVector TraceStart = CameraComp->GetComponentLocation();
		FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * 5000);

		FHitResult HitResult;
		// 执行碰撞查询，检查是否有物体位于起始点到终点之间，如果有碰撞发生，则将结果存储在HitResult里面
		if (GetWorld()->SweepSingleByObjectType(HitResult , TraceStart , TraceEnd , \
					FQuat::Identity , CollsionQueryParams , CollisionShape , CollisionParams))
		{
			// 如果发生碰撞，将 TraceEnd 更新为碰撞点的位置，以确保投射物生成在碰撞点处
			TraceEnd = HitResult.ImpactPoint;
		}

		// 根据手部位置和碰撞点位置，根据X轴单位向量计算旋转矩阵生成投射物的旋转角度
		FRotator ProjRotate = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		FTransform SpawnTM = FTransform(ProjRotate , HandLocation);

		// 生成对应的Actor类
		GetWorld()->SpawnActor<AActor>(ClassToSpawn , SpawnTM , SpawnParams);

	}
}

void APersonCharacter::AttackEffects()
{
	PlayAnimMontage(AttackAnim); // 添加攻击动画

	// 在角色的骨骼或Mesh上附加一个粒子效果，ZeroVector和ZeroRotator表示和插座的位置及旋转角度一致
	// SnapToTarget表示粒子效果紧密贴在插座的位置
	UGameplayStatics::SpawnEmitterAttached(CastingEffect , GetMesh() , TEXT("") , \
						FVector::ZeroVector , FRotator::ZeroRotator , EAttachLocation::SnapToTarget);
}

void APersonCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass , FName("Muzzle_01"));
}

void APersonCharacter::PrimaryAttack()
{
	AttackEffects();

	// 添加计时器产生延迟避免在抬手前就已经释放魔法弹，0.2秒后才触发
	// 方法二是使用动画事件 TODO
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack , \
			this , &APersonCharacter::PrimaryAttack_TimeElapsed , AttackAnimDelay);
	
	// 清除掉计时器
	// GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}


void APersonCharacter::BlackHoleAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass , FName("Muzzle_01"));
}

void APersonCharacter::BlackHoleAttack()
{
	AttackEffects();

	GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAttack , \
			this , &APersonCharacter::BlackHoleAttack_TimeElapsed , AttackAnimDelay);
}

void APersonCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass , FName("Muzzle_01"));
}

void APersonCharacter::Dash()
{
	AttackEffects();

	GetWorldTimerManager().SetTimer(TimerHandle_Dash , \
			this , &APersonCharacter::Dash_TimeElapsed , AttackAnimDelay);
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
	PlayerInputComponent->BindAxis("MoveForward" , this , &APersonCharacter::MoveForward); // WS
	PlayerInputComponent->BindAxis("MoveRight" , this , &APersonCharacter::MoveRight); // AD
	
	// 绑定Yaw实现左右水平偏航，Pitch是俯仰倾斜旋转，Roll是侧翻滚动
	// Turn和LookUp绑定的函数是由APawn中已经写好的内容
	PlayerInputComponent->BindAxis("Turn" , this , &APawn::AddControllerYawInput); // MouseX
	PlayerInputComponent->BindAxis("LookUp" , this , &APawn::AddControllerPitchInput); // MouseY

	// 绑定动作实现攻击发射
	PlayerInputComponent->BindAction("PrimaryAttack" , IE_Pressed , this , &APersonCharacter::PrimaryAttack); // 魔法弹-Left Mouse Button 
	PlayerInputComponent->BindAction("BlackholeAttack" , IE_Pressed , this , &APersonCharacter::BlackHoleAttack); // 黑洞-E
	PlayerInputComponent->BindAction("Dash" , IE_Pressed , this , &APersonCharacter::Dash); // 瞬移-C

	// 绑定空格键实现角色的跳跃、shift键实现角色的下蹲
	PlayerInputComponent->BindAction("MoveJump" , IE_Pressed , this , &APersonCharacter::MoveJump); // Space
	PlayerInputComponent->BindAction("MoveJump" , IE_Released , this , &APersonCharacter::StopJump); // TODO
	PlayerInputComponent->BindAction("Crouch" , IE_Pressed , this , &APersonCharacter::ToggleCrouch); // TODO
	PlayerInputComponent->BindAction("Crouch" , IE_Released , this , &APersonCharacter::StopCrouch); // TODO
	
	// 绑定E键位实现与其他Actor的交互
	PlayerInputComponent->BindAction("PrimaryInteract" , IE_Pressed , this , &APersonCharacter::PrimaryInteract);
}

