// Fill out your copyright notice in the Description page of Project Settings.

#include "BombermanTestGameModeBase.h"
#include "Engine.h"
#include "Engine/World.h"

ABombermanTestGameModeBase * ABombermanTestGameModeBase::instance = nullptr; // Initialize the singleton instance to nullptr

void ABombermanTestGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	// Singleton pattern to quickly access this game mode
	instance = this;

}

void ABombermanTestGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (gameActive)
	{

		timeLeft -= DeltaTime;

		if (timeLeft <= 0 || alivePlayers < 2)
		{
			EndGame();
		}
	}

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

	// Create a second local player.  Must do this manually, because Unreal Engine wants to spawn second
	// players in another window, rather than on the same screen.
	UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
	timeLeft = roundTime;
	alivePlayers = 2;
	gameActive = true;

}

void ABombermanTestGameModeBase::ExplodeBombAt(int32 x, int32 y, ABombermanPlayer * player, int32 range)
{
	mapGenerator->DoBombAt(x, y, range);
}

void ABombermanTestGameModeBase::PlayerKilled()
{
	alivePlayers--;
}

void ABombermanTestGameModeBase::EndGame()
{
	gameActive = false;

	if (alivePlayers == 1)
	{
		TArray<AActor *> outActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABombermanPlayer::StaticClass(), outActors);

		if (outActors.Num() > 0)
		{
			ABombermanPlayer *livingPlayer = Cast<ABombermanPlayer>(outActors[0]);
			TriggerGameOver(alivePlayers, livingPlayer);
		}
		else
		{
			TriggerGameOver(0, nullptr);  // DRAW - both players dead.
		}

	}
	else
	{
		TriggerGameOver(alivePlayers, nullptr); // DRAW - Time ran out
	}

}

void ABombermanTestGameModeBase::TriggerGameOver_Implementation(int32 numAlivePlayers, ABombermanPlayer *winner) const
{
	// Must also manually do the reverse of the second local player creation. Remove the player from the game
	// so that on restart, we don't have conflicting player counts.
	APlayerController * player = UGameplayStatics::GetPlayerController(GetWorld(), 1);
	UGameplayStatics::RemovePlayer(player, false);
}



