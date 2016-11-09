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

void AMainCharacterPlayerController::PlayerTick(float deltaTime)
{
	//Super::PlayerTick(deltaTime);

	// keep updating the destination every tick while desired
	/*if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}*/

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

	
}
//
//void AMainCharacterPlayerController::SetupInputComponent()
//{
//	// set up gameplay key bindings
//	//Super::SetupInputComponent();
//
//	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AMainCharacterPlayerController::OnSetDestinationPressed);
//	InputComponent->BindAction("SetDestination", IE_Released, this, &AMainCharacterPlayerController::OnSetDestinationReleased);
//
//	// support touch devices 
//	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AMainCharacterPlayerController::MoveToTouchLocation);
//	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AMainCharacterPlayerController::MoveToTouchLocation);
//
//	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AMainCharacterPlayerController::OnResetVR);
//}

