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
	isRotating = false;
	rXAxis = 0.0f;
	rYAxis = 0.0f;
	turnRate = 5.0f;
	dodgePotency = 2500.0f;
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

void AMainCharacterPlayerController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	//  Get the scale of analog input
	FVector2D v2 = FVector2D(xMoveDir, yMoveDir);
	float axisScale = FMath::Clamp(v2.Size(), 0.0f, 1.0f);

	PerformMovement(axisScale);
	PerformRotation(axisScale);

	/*if (isInCombat && isBlocking == false && isDodging == false && isAttacking == false)
	{
		inCombatTimer -= deltaTime;

		if (inCombatTimer <= 0)
		{
			isInCombat = false;
		}
	}*/

	isInCombat = true;
}

void AMainCharacterPlayerController::PerformMovement(float axisScale)
{
	// Multiply the analog input axis by the scale of our speed
	speed = axisScale * speedScale;

	if (isAttacking != true)
	{
		AddMovementInput(FVector(yMoveDir, xMoveDir, 0.0f), axisScale, false);
	}
}

void AMainCharacterPlayerController::PerformRotation(float axisScale)
{
	// Check if right analog has any input
	if (rXAxis + rYAxis == 0)
	{
		isRotating = false;
	}
	else
	{
		isRotating = true;
	}

	float x, y, targetAngle;

	// If right analog stick has any input, have character face that direction
	// Otherwise have character face the direction they are moving
	if (isRotating)
	{
		x = rXAxis;
		y = rYAxis;
	}
	else
	{
		x = xMoveDir;
		y = yMoveDir;
	}

	targetAngle = FMath::Atan2(x, y) * 180.0f / PI;

	Controller->GetControlRotation();

	if (Controller)
	{
		float yaw;
		FRotator rotator;
		float deltaTurn;

		rotator = Controller->GetControlRotation();

		// If right analog stick has any input, have character turn to that direction ASAP
		// Otherwise have character turn to the direction he is moving in relative to the speed he is moving
		if (isRotating)
		{
			deltaTurn = turnRate;
		}
		else
		{
			deltaTurn = axisScale * turnRate;
		}

		//FString::SanitizeFloat(deltaTurn)
		////UE_LOG(LogTemp, Warning, TEXT());
		//UE_LOG(LogTemp, Warning, TEXT("%f"), deltaTurn);

		yaw = FMath::FixedTurn(Controller->GetControlRotation().Yaw, targetAngle, axisScale * turnRate);
		rotator.Yaw = yaw;

		Controller->SetControlRotation(rotator);
	}

	// Set Face Direction values from Controllers Rotation
	xFaceDir = FMath::Sin(Controller->GetControlRotation().Yaw * PI / 180.0f);
	yFaceDir = FMath::Cos(Controller->GetControlRotation().Yaw * PI / 180.0f);

	float faceAngle, moveAngle, deltaAngle;

	faceAngle = FMath::Atan2(xFaceDir, yFaceDir) * 180.0f / PI;
	moveAngle = FMath::Atan2(xMoveDir, yMoveDir) * 180.0f / PI;

	if (axisScale == 0.0f)
	{
		xMoveRelativeToFaceDir = 0.0f;
		yMoveRelativeToFaceDir = 0.0f;
	}
	else
	{
		deltaAngle = faceAngle - moveAngle;

		if (deltaAngle < 0.0f)
		{
			deltaAngle += 360.0f;
		}

		/*if (deltaAngle == 0.0f)
		{
			
		}*/
		UE_LOG(LogTemp, Warning, TEXT("%f"), deltaAngle);

		xMoveRelativeToFaceDir = FMath::Sin(deltaAngle * PI / 180.0f) * axisScale;
		yMoveRelativeToFaceDir = FMath::Cos(deltaAngle * PI / 180.0f) * axisScale;
	}
}

void AMainCharacterPlayerController::AddXMovement(float axisValue)
{
	xMoveDir = axisValue;
}

void AMainCharacterPlayerController::AddYMovement(float axisValue)
{
	yMoveDir = axisValue;
}

void AMainCharacterPlayerController::AddXRotation(float axisValue)
{
	rXAxis = axisValue;
	//xFaceDir = FMath::Clamp(-1.0f, 1.0f, xFaceDir + axisValue);
}

void AMainCharacterPlayerController::AddYRotation(float axisValue)
{
	rYAxis = -axisValue;
	//yFaceDir = FMath::Clamp(-1.0f, 1.0f, yFaceDir + axisValue);
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

	//AddMovementInput(FVector(0.0f, 1.0f, 0.0f), dodgePotency, true);

	float x, y;
	if (xMoveDir + yMoveDir == 0.0f)
	{
		x = xFaceDir; // FMath::Sin(Controller->GetControlRotation().Yaw * PI / 180.0f);
		y = yFaceDir; // FMath::Cos(Controller->GetControlRotation().Yaw * PI / 180.0f);
	}
	else
	{
		x = xMoveDir;
		y = yMoveDir;
	}

	LaunchCharacter(FVector(y * dodgePotency, x * dodgePotency, 0.0f), true, false);
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