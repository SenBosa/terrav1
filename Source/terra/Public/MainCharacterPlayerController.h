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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	float speedScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	float xMoveDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	float yMoveDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	float xFaceDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	float yFaceDir;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	bool isInCombat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	bool isAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	bool isBlocking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	bool isDodging;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	int attackIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	float inCombatDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	float inCombatTimer;

protected:

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	/*virtual*/ void PlayerTick(float DeltaTime) /*override*/;
	///*virtual*/ void SetupInputComponent() /*override*/;
	// End PlayerController interface

	
	
};
