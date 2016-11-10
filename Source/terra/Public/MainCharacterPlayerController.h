// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
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

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	void EnterCombat();
};
