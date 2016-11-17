// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "MainCharacterPlayerController.generated.h"

UENUM(BlueprintType)
enum class CharacterState : uint8
{
	IDLE,
	IDLE_COMBAT,
	ATTACKING,
	BLOCKING,
	DODGING
};

/**
 * 
 */
UCLASS()
class TERRA_API AMainCharacterPlayerController : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacterPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enum)
	CharacterState state;

	// Speed variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	float speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	float speedScale;

	// Move and face values (analog input)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	float xMoveDir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	float yMoveDir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	float xFaceDir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	float yFaceDir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	float xMoveRelativeToFaceDir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	float yMoveRelativeToFaceDir;
	float rXAxis;
	float rYAxis;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	bool isRotating;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	float turnRate;
	
	// Combat variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	bool isInCombat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerData)
	float inCombatDuration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	float inCombatTimer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	bool isAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	int attackIndex;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	bool isBlocking;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	bool isDodging;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerData)
	float dodgeTimer;
	float dodgePotency;
	float dodgeBeginDelay;
	float dodgeEndDelay;
	float dodgeDuration;
	bool hasDodged;
	float xDodge;
	float yDodge;

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	void Idle(float deltaTime, float axisScale);
	void Attacking(float deltaTime, float axisScale);
	void Blocking(float deltaTime, float axisScale);
	void Dodging(float deltaTime, float axisScale);

	void PerformMovement(float axisScale);
	void PerformRotation(float axisScale);

	void AddXMovement(float axisValue);
	void AddYMovement(float axisValue);
	void AddXRotation(float axisValue);
	void AddYRotation(float axisValue);
	
	void OnSpell1Use();
	void OnSpell2Use();
	void OnSpell3Use();
	void ActivateBlocking();
	void DeactivateBlocking();
	void ActivateDodging();
	void DeactivateDodging();
	void ActivateLightAttack();
	void DeactivateLightAttack();
	void ActivateHeavyAttack();
	void DeactivateHeavyAttack();
	void EnterCombat(CharacterState newState);
};
