// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonCharacter.h"

// Sets default values
APersonCharacter::APersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

}

