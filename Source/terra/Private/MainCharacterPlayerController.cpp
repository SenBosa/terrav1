// Fill out your copyright notice in the Description page of Project Settings.

#include "terra.h"
#include "MainCharacterPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
//#include "TP_TopDownCharacter.h"



AMainCharacterPlayerController::AMainCharacterPlayerController()
{
	//bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::Crosshairs;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	speed = 0.0f;
	speedScale = 400.0f;
	xMoveDir = 0.0f;
	yMoveDir = 0.0f;
	xFaceDir = 0.0f;
	yFaceDir = 0.0f;
	isInCombat = false;
	isAttacking = false;
	isBlocking = false;
	isDodging = false;
	attackIndex = 0;
	inCombatDuration = 5.0f;
	inCombatTimer = 0.0f;
}

void AMainCharacterPlayerController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	FVector2D v2 = FVector2D(xMoveDir, yMoveDir);
	speed = FMath::Clamp(v2.Size() * speedScale, 0.0f, 400.0f);

	if (isInCombat)
	{
		inCombatTimer -= deltaTime;

		if (inCombatTimer <= 0)
		{
			isInCombat = false;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Tick"));
}

void AMainCharacterPlayerController::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//InputComponent->BindAction("SetDestination", IE_Pressed, this, &AMainCharacterPlayerController::OnSetDestinationPressed);
	//InputComponent->BindAction("SetDestination", IE_Released, this, &AMainCharacterPlayerController::OnSetDestinationReleased);

	UE_LOG(LogTemp, Warning, TEXT("Happens"));

	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacterPlayerController::AddXMovement);
	PlayerInputComponent->BindAxis("MoveUp", this, &AMainCharacterPlayerController::AddYMovement);
	PlayerInputComponent->BindAxis("TurnLeftRight", this, &AMainCharacterPlayerController::AddXRotation);
	PlayerInputComponent->BindAxis("TurnUpDown", this, &AMainCharacterPlayerController::AddYRotation);

	PlayerInputComponent->BindAction("Spell1", IE_Pressed, this, &AMainCharacterPlayerController::OnSpell1Use);
	PlayerInputComponent->BindAction("Spell2", IE_Pressed, this, &AMainCharacterPlayerController::OnSpell2Use);
	PlayerInputComponent->BindAction("Spell3", IE_Pressed, this, &AMainCharacterPlayerController::OnSpell3Use);
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AMainCharacterPlayerController::ActivateBlocking);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &AMainCharacterPlayerController::DeactivateBlocking);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AMainCharacterPlayerController::ActivateDodging);
	PlayerInputComponent->BindAction("Dodge", IE_Released, this, &AMainCharacterPlayerController::DeactivateDodging);
	PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &AMainCharacterPlayerController::ActivateLightAttack);
	PlayerInputComponent->BindAction("LightAttack", IE_Released, this, &AMainCharacterPlayerController::DeactivateLightAttack);
	PlayerInputComponent->BindAction("HeavyAttack", IE_Pressed, this, &AMainCharacterPlayerController::ActivateHeavyAttack);
	PlayerInputComponent->BindAction("HeavyAttack", IE_Released, this, &AMainCharacterPlayerController::DeactivateHeavyAttack);
}

void AMainCharacterPlayerController::AddXMovement(float axisValue)
{
	xMoveDir = axisValue;

	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), xMoveDir, false);
}

void AMainCharacterPlayerController::AddYMovement(float axisValue)
{
	yMoveDir = axisValue;

	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), yMoveDir, false);
}

void AMainCharacterPlayerController::AddXRotation(float axisValue)
{
	xFaceDir = FMath::Clamp(-1.0f, 1.0f, xFaceDir + axisValue);
}

void AMainCharacterPlayerController::AddYRotation(float axisValue)
{
	yFaceDir = FMath::Clamp(-1.0f, 1.0f, yFaceDir + axisValue);
}

void AMainCharacterPlayerController::OnSpell1Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Spell1"));
}

void AMainCharacterPlayerController::OnSpell2Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Spell2"));
}

void AMainCharacterPlayerController::OnSpell3Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Spell3"));
}

void AMainCharacterPlayerController::ActivateBlocking()
{
	EnterCombat();
	isBlocking = true;
}

void AMainCharacterPlayerController::DeactivateBlocking()
{
	isBlocking = false;
}

void AMainCharacterPlayerController::ActivateDodging()
{
	EnterCombat();
	isDodging = true;
}

void AMainCharacterPlayerController::DeactivateDodging()
{
	isDodging = false;
}

void AMainCharacterPlayerController::ActivateLightAttack()
{
	EnterCombat();
	attackIndex = 0;
	isAttacking = true;
}

void AMainCharacterPlayerController::DeactivateLightAttack()
{
	isAttacking = false;
}

void AMainCharacterPlayerController::ActivateHeavyAttack()
{
	EnterCombat();
	attackIndex = 1;
	isAttacking = true;
}

void AMainCharacterPlayerController::DeactivateHeavyAttack()
{
	isAttacking = false;
}

void AMainCharacterPlayerController::EnterCombat()
{
	isInCombat = true;
	inCombatTimer = inCombatDuration;
}