// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EnemyAI.h"
#include "EnemyAISpawner.generated.h"

UCLASS()
class TERRA_API AEnemyAISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyAISpawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnData)
	int numEnemies;
	const int NUM_ENEMIES = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnData)
	float spawnTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnData)
	float spawnFrequency;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnData)
	AEnemyAI enemies[5];
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnData)
	AActor spawnPoints[5];

	/*UFUNCTION()
	AEnemyAI* GetEnemies();*/
};
