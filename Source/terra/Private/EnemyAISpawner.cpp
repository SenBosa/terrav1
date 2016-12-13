// Fill out your copyright notice in the Description page of Project Settings.

#include "terra.h"
#include "EnemyAISpawner.h"


// Sets default values
AEnemyAISpawner::AEnemyAISpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyAISpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyAISpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

//AEnemyAI* AEnemyAISpawner::GetEnemies()
//{
//	
//	if (enemies != NULL)
//	{
//		return enemies;
//	}
//	else return nullptr;
//}