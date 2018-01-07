// Fill out your copyright notice in the Description page of Project Settings.

#include "BombermanTestGameModeBase.h"
#include "Engine.h"

ABombermanTestGameModeBase * ABombermanTestGameModeBase::instance = nullptr; // Initialize the singleton instance to nullptr

void ABombermanTestGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	// Singleton pattern to quickly access this game mode
	instance = this;
	StartGame();
}

void ABombermanTestGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ABombermanTestGameModeBase::StartGame()
{
	FVector location = FVector(0, 0, 0);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.bDeferConstruction = false;

	mapGenerator = GetWorld()->SpawnActor<AMapGenerator>(MapGeneratorActor, location, FRotator::ZeroRotator, SpawnInfo);
	mapGenerator->GenerateMapData();
	mapGenerator->GenerateMapGeometry();

}

