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
	chaseRange = 20.0f;
	attackRange = 5.0f;

	playerCharacter = NULL;
}

// Called every frame
void AEnemyAI::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	CheckMovement();

	switch (state)
	{
	case EnemyState::IDLE:
	case EnemyState::IDLE_COMBAT:
		Idle(deltaTime);
		break;
	case EnemyState::CHASING:
		PerformMovement();
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

	FVector playerLocation = playerCharacter->GetActorLocation();
	FVector myLocation = GetActorLocation();
	FVector distanceVector = playerLocation - myLocation;
	float distance = distanceVector.Size();

	if (distance <= chaseRange)
	{
		state = EnemyState::CHASING;
	}
}

void AEnemyAI::Attacking(float deltaTime)
{
	FVector playerLocation = playerCharacter->GetActorLocation();
	FVector myLocation = GetActorLocation();
	FVector distanceVector = playerLocation - myLocation;
	float distance = distanceVector.Size();

	if (distance > attackRange)
	{
		state = EnemyState::CHASING;
	}
}

void AEnemyAI::PerformMovement()
{
	FVector playerLocation = playerCharacter->GetActorLocation();
	FVector myLocation = GetActorLocation();
	FVector distanceVector = playerLocation - myLocation;
	float distance = distanceVector.Size();

	// Multiply the analog input axis by the scale of our speed
	speed = speedScale;

	if (isAttacking != true)
	{
		AddMovementInput(FVector(yMoveDir, xMoveDir, 0.0f), 1.0f, false);

		if (distance <= attackRange)
		{
			state = EnemyState::ATTACKING;
		}
	}

	if (distance > chaseRange)
	{
		state = EnemyState::IDLE;
	}
}

void AEnemyAI::CheckMovement()
{
	FVector playerLocation = playerCharacter->GetActorLocation();
	FVector myLocation = GetActorLocation();

	FVector direction = playerLocation - myLocation;

	direction.Normalize();
	xMoveDir = direction.X;
	yMoveDir = direction.Y;
}

void AEnemyAI::AddPlayer(APawn* player)
{
	playerCharacter = player;
}
