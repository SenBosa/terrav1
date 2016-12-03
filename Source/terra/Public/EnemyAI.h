// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EnemyAI.generated.h"

UENUM(BlueprintType)
enum class EnemyState : uint8
{
	IDLE,
	IDLE_COMBAT,
	CHASING,
	ATTACKING,
};

UCLASS()
class TERRA_API AEnemyAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAI();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enum)
	EnemyState state;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Speed variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyData)
	float speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyData)
	float speedScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyData)
	float turnRate;

	// Move and face values (analog input)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyData)
	float xMoveDir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyData)
	float yMoveDir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyData)
	float xFaceDir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyData)
	float yFaceDir;

	// Combat variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyData)
	bool isAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyData)
	float chaseRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyData)
	float attackRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyData)
	float attackTimer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyData)
	float attackDuration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyData)
	float attackWindUpDuration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyData)
	bool alternateAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyData)
	APawn* playerCharacter;

	void Idle(float deltaTime);
	void Attacking(float deltaTime);
	void CheckMovement();
	void PerformMovement();
	void PerformRotation();
	void AddPlayer(APawn* player);

protected:

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
