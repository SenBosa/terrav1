// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MainCharacterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TERRA_API AMainCharacterPlayerController : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacterPlayerController();

	UPROPERTY(EditAnywhere)
	float speed;

	UPROPERTY(EditAnywhere)
	float speedScale;

	UPROPERTY(EditAnywhere)
	float xMoveDir;

	UPROPERTY(EditAnywhere)
	float yMoveDir;

	UPROPERTY(EditAnywhere)
	float xFaceDir;

	UPROPERTY(EditAnywhere)
	float yFaceDir;
	
	UPROPERTY(EditAnywhere)
	bool isInCombat;

	UPROPERTY(EditAnywhere)
	bool isAttacking;

	UPROPERTY(EditAnywhere)
	bool isBlocking;

	UPROPERTY(EditAnywhere)
	bool isDodging;

	UPROPERTY(EditAnywhere)
	int attackIndex;

	UPROPERTY(EditAnywhere)
	float inCombatDuration;

	UPROPERTY(EditAnywhere)
	float inCombatTimer;

protected:

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	/*virtual*/ void PlayerTick(float DeltaTime) /*override*/;
	///*virtual*/ void SetupInputComponent() /*override*/;
	// End PlayerController interface

	
	
};
