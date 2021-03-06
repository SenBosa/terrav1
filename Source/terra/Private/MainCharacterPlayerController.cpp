// Fill out your copyright notice in the Description page of Project Settings.

#include "terra.h"
#include "MainCharacterPlayerController.h"
#include "EnemyAI.h"
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
	attackTimer = 0.0f;
	attack1Duration = 1.1113f;//1.667f;
	attack1WindUpDuration = 0.44467f;
	attack1SwingDuration = 0.667f;
	attack2Duration = 0.667f;
	attack2WindUpDuration = 0.333f;
	attack2SwingDuration = 0.6f;
	attack3Duration = 1.0f;
	attack3WindUpDuration = 0.167f;
	attack3SwingDuration = 0.5f;
	promptAttack2 = false;
	promptAttack3 = false;

	inCombatDuration = 5.0f;
	inCombatTimer = 0.0f;
	isRotating = false;
	rXAxis = 0.0f;
	rYAxis = 0.0f;
	turnRate = 10.0f;
	dodgePotency = 1000.0f;
	dodgeTimer = 0.0f;
	dodgeBeginDelay = 0.1f;
	dodgeDuration = 0.333333f;
	dodgeEndDelay = 0.15f;
	hasDodged = false;
	xDodge = 0.0f;
	yDodge = 0.0f;

	health = 3;
	staggerPotency = 1000.0f;
	//AMainCharacterPlayerController::instance = this;
	addedCapsule = false;
}

void AMainCharacterPlayerController::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//InputComponent->BindAction("SetDestination", IE_Pressed, this, &AMainCharacterPlayerController::OnSetDestinationPressed);
	//InputComponent->BindAction("SetDestination", IE_Released, this, &AMainCharacterPlayerController::OnSetDestinationReleased);

	//UE_LOG(LogTemp, Warning, TEXT("Happens"));

	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacterPlayerController::AddXMovement);
	PlayerInputComponent->BindAxis("MoveUp", this, &AMainCharacterPlayerController::AddYMovement);
	PlayerInputComponent->BindAxis("TurnLeftRight", this, &AMainCharacterPlayerController::AddXRotation);
	PlayerInputComponent->BindAxis("TurnUpDown", this, &AMainCharacterPlayerController::AddYRotation);

	PlayerInputComponent->BindAction("Spell1", IE_Pressed, this, &AMainCharacterPlayerController::OnSpell1Use);
	PlayerInputComponent->BindAction("Spell2", IE_Pressed, this, &AMainCharacterPlayerController::OnSpell2Use);
	PlayerInputComponent->BindAction("Spell3", IE_Pressed, this, &AMainCharacterPlayerController::OnSpell3Use);
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AMainCharacterPlayerController::ActivateBlocking);
	//PlayerInputComponent->BindAction("Block", IE, this, &AMainCharacterPlayerController::ActivateBlocking);
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

	if (addedCapsule == false)
	{
		if (weaponCapsule != NULL)
		{
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *weaponCapsule->GetName());
			weaponCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacterPlayerController::OnWeaponHit);
			//weaponCapsule->BeginComponentOverlap.AddDynamic(this, &AMainCharacterPlayerController::OnWeaponHit);
			addedCapsule = true;
		}
	}

	//  Get the scale of analog input
	FVector2D v2 = FVector2D(xMoveDir, yMoveDir);
	float axisScale = FMath::Clamp(v2.Size(), 0.0f, 1.0f);

	switch (state)
	{
	case CharacterState::IDLE:
	case CharacterState::IDLE_COMBAT:
		Idle(deltaTime, axisScale);
		break;
	case CharacterState::ATTACKING:
		Attacking(deltaTime, axisScale);
		break;
	case CharacterState::BLOCKING:
		Blocking(deltaTime, axisScale);
		break;
	case CharacterState::DODGING:
		Dodging(deltaTime, axisScale);
		break;
	default:
		break;
	}

	if (isInCombat && isBlocking == false && isDodging == false && isAttacking == false)
	{
		inCombatTimer -= deltaTime;

		if (inCombatTimer <= 0)
		{
			isInCombat = false;
		}
	}

	//isInCombat = true;
}

void AMainCharacterPlayerController::Idle(float deltaTime, float axisScale)
{
	PerformMovement(axisScale);
	PerformRotation(axisScale);
}

void AMainCharacterPlayerController::Attacking(float deltaTime, float axisScale)
{
	attackTimer += deltaTime;

	switch (attackIndex)
	{
	case 0:
		if (promptAttack2)
		{
			if (attackTimer >= attack1SwingDuration)
			{
				attackIndex = 1;
				attackTimer = 0.0f;
			}
		}
		else
		{
			if (attackTimer >= attack1Duration)
			{
				StopAttacking();
			}
		}
		break;
	case 1:
		if (promptAttack3)
		{
			if (attackTimer >= attack2SwingDuration)
			{
				attackIndex = 2;
				attackTimer = 0.0f;
			}
		}
		else
		{
			if (attackTimer >= attack2Duration)
			{
				StopAttacking();
			}
		}
		break;
	case 2:
		if (attackTimer >= attack3Duration)
		{
			StopAttacking();
		}
		break;
	}
}

void AMainCharacterPlayerController::Blocking(float deltaTime, float axisScale)
{
	PerformMovement(axisScale);
	PerformRotation(axisScale);
}

void AMainCharacterPlayerController::Dodging(float deltaTime, float axisScale)
{
	if (isAttacking == false)
	{
		if (hasDodged == false)
		{
			PerformRotation(axisScale);
		}

		dodgeTimer += deltaTime;

		if (dodgeTimer >= dodgeBeginDelay)
		{
			if (hasDodged == false)
			{
				if (xMoveDir + yMoveDir == 0.0f)
				{
					xDodge = xFaceDir; // FMath::Sin(Controller->GetControlRotation().Yaw * PI / 180.0f);
					yDodge = yFaceDir; // FMath::Cos(Controller->GetControlRotation().Yaw * PI / 180.0f);
				}
				else
				{
					xDodge = xMoveDir;
					yDodge = yMoveDir;
				}

				hasDodged = true;
			}

			if (dodgeTimer >= dodgeBeginDelay + dodgeDuration)
			{
				//hasDodged = true;
				isDodging = false;
			}
			else
			{
				SetActorLocation(GetActorLocation() + FVector(yDodge, xDodge, 0.0f) * dodgePotency * deltaTime);
			}

			if (hasDodged == true && dodgeTimer >= dodgeBeginDelay + dodgeDuration + dodgeEndDelay)
			{
				//isDodging = false;
				state = CharacterState::IDLE_COMBAT;
			}
		}
	}
}

void AMainCharacterPlayerController::PerformMovement(float axisScale)
{
	if (isBlocking == true)
	{
		axisScale *= 0.5f;
	}

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

		yaw = FMath::FixedTurn(rotator.Yaw, targetAngle, axisScale * turnRate);
		rotator.Yaw = yaw;

		Controller->SetControlRotation(rotator);
	}

	// Set Face Direction values from Controllers Rotation
	xFaceDir = FMath::Sin(Controller->GetControlRotation().Yaw * PI / 180.0f);
	yFaceDir = FMath::Cos(Controller->GetControlRotation().Yaw * PI / 180.0f);

	float faceAngle, moveAngle, deltaAngle;

	faceAngle = FMath::Atan2(xFaceDir, yFaceDir) * 180.0f / PI;
	moveAngle = FMath::Atan2(xMoveDir, yMoveDir) * 180.0f / PI;

	if (axisScale == 0.0f && isDodging == false)
	{
		xMoveRelativeToFaceDir = 0.0f;
		yMoveRelativeToFaceDir = 0.0f;
	}
	else if (axisScale == 0.0f && isDodging == true)
	{
		xMoveRelativeToFaceDir = 0.0f;
		yMoveRelativeToFaceDir = 1.0f;
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
		//UE_LOG(LogTemp, Warning, TEXT("%f"), deltaAngle);

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
	if (state == CharacterState::IDLE || state == CharacterState::IDLE_COMBAT)
	{
		EnterCombat(CharacterState::BLOCKING);
		isBlocking = true;
	}
}

void AMainCharacterPlayerController::DeactivateBlocking()
{
	if (state == CharacterState::BLOCKING)
	{
		isBlocking = false;
		state = CharacterState::IDLE_COMBAT;
	}
}

void AMainCharacterPlayerController::ActivateDodging()
{
	if (state == CharacterState::IDLE || state == CharacterState::IDLE_COMBAT)
	{
		if (state != CharacterState::DODGING)
		{
			isDodging = true;
			hasDodged = false;
			dodgeTimer = 0.0f;
			EnterCombat(CharacterState::DODGING);
		}
	}
}

void AMainCharacterPlayerController::DeactivateDodging()
{
	//isDodging = false;
	//state = CharacterState::IDLE_COMBAT;
}

void AMainCharacterPlayerController::ActivateLightAttack()
{
	if (state != CharacterState::DODGING && state != CharacterState::BLOCKING)
	{
		if (isAttacking == false)
		{
			EnterCombat(CharacterState::ATTACKING);
			isAttacking = true;
			attackIndex = 0;
			attackTimer = 0.0f;
		}
		else if (promptAttack2 == false || promptAttack3 == false)
		{
			EnterCombat(CharacterState::ATTACKING);
			if (attackIndex == 0)
			{
				if (promptAttack2 == false)
				{
					promptAttack2 = true;
				}
				else
				{
					promptAttack3 = true;
				}
			}
			if (attackIndex == 1)
			{
				promptAttack3 = true;
			}
		}
	}
}

void AMainCharacterPlayerController::DeactivateLightAttack()
{
	/*isAttacking = false;
	state = CharacterState::IDLE_COMBAT;*/
}

void AMainCharacterPlayerController::ActivateHeavyAttack()
{
	/*EnterCombat(CharacterState::ATTACKING);
	attackIndex = 1;
	isAttacking = true;*/
}

void AMainCharacterPlayerController::DeactivateHeavyAttack()
{
	/*isAttacking = false;
	state = CharacterState::IDLE_COMBAT;*/
}

void AMainCharacterPlayerController::EnterCombat(CharacterState newState)
{
	isInCombat = true;
	inCombatTimer = inCombatDuration;
	state = newState;
}

void AMainCharacterPlayerController::StopAttacking()
{
	isAttacking = false;
	attackIndex = 0.0f;
	promptAttack2 = false;
	promptAttack3 = false;
	state = CharacterState::IDLE_COMBAT;
}

void AMainCharacterPlayerController::OnDamageTaken(FVector enemyPosition)
{
	if (isDodging == false)
	{
		FVector v = GetActorLocation() - enemyPosition;
		v.Normalize();
		v *= staggerPotency;

		if (isBlocking)
		{
			v /= 5.0f;
		}
		else
		{
			health--;
		}


		LaunchCharacter(v, true, false);
	}
}

void AMainCharacterPlayerController::OnWeaponHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (OtherActor->GetClass()->IsChildOf(AEnemyAI::StaticClass()))
		{
			AEnemyAI* ai = (AEnemyAI*)OtherActor;
			if (ai != NULL)
			{
				//UE_LOG(LogTemp, Warning, TEXT("%s"), ai->GetName());
				//UE_LOG(LogTemp, Warning, TEXT("triggered"));


				switch (attackIndex)
				{
				case 0:
					if (isAttacking && attackTimer >= attack1WindUpDuration && attackTimer <= attack1SwingDuration)
					{
						ai->TakeDamageAndStagger(GetActorLocation(), staggerPotency / 5.0f);
					}
					break;
				case 1:
					if (isAttacking && attackTimer >= attack2WindUpDuration && attackTimer <= attack2SwingDuration)
					{
						ai->TakeDamageAndStagger(GetActorLocation(), staggerPotency / 5.0f);
					}
					break;
				case 2:
					if (isAttacking && attackTimer >= attack3WindUpDuration && attackTimer <= attack3SwingDuration)
					{
						ai->TakeDamageAndStagger(GetActorLocation(), staggerPotency);
					}
					break;
				}
			}
		}
	}
}