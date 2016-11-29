// Fill out your copyright notice in the Description page of Project Settings.

#include "terra.h"
#include "EnemyAI.h"


// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	
	speed = 0.0f;
	speedScale = 400.0f;
	xMoveDir = 0.0f;
	yMoveDir = 0.0f;
	xFaceDir = 0.0f;
	yFaceDir = 0.0f;
	isAttacking = false;

	playerCharacter = NULL;
}

// Called every frame
void AEnemyAI::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	switch (state)
	{
	case EnemyState::IDLE:
	case EnemyState::IDLE_COMBAT:
		Idle(deltaTime);
		break;
	case EnemyState::ATTACKING:
		Attacking(deltaTime);
		break;
	default:
		break;
	}
}

//// Called to bind functionality to input
//void AEnemyAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

void AEnemyAI::Idle(float deltaTime)
{
	/*PerformMovement(axisScale);
	PerformRotation(axisScale);*/
}

void AEnemyAI::Attacking(float deltaTime)
{

}

void AEnemyAI::PerformMovement()
{
	// Multiply the analog input axis by the scale of our speed
	speed = speedScale;

	if (isAttacking != true)
	{
		AddMovementInput(FVector(yMoveDir, xMoveDir, 0.0f), 1.0f, false);
	}
}

void AEnemyAI::CheckMovement()
{
	FVector playerLocation = playerCharacter->GetActorLocation();
	FVector myLocation = GetActorLocation();

	FVector direction = playerLocation - myLocation;
}

void AEnemyAI::AddPlayer(APawn* player)
{
	playerCharacter = player;
}
