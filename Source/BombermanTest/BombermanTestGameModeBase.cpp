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

	// Singleton to quickly access this game mode
	instance = this;

}

void ABombermanTestGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (gameActive)
	{
		timeLeft -= DeltaTime;

		// End game conditions - timer is finished, OR there are no living players.
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
	if (!useAI)
	{
		UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
	}
	else
	{
		// TODO: Make this position NOT a magic number.
		GetWorld()->SpawnActor<ABombermanPlayer>(AIPlayer, FVector(700.0f, 700.0f, 200.0f), FRotator::ZeroRotator);
	}

	timeLeft = roundTime;
	alivePlayers = 2;
	gameActive = true;

}

// Talks to the map generator and tells it to manage the explosion in the maze.
void ABombermanTestGameModeBase::ExplodeBombAt(int32 x, int32 y, ABombermanPlayer * player, int32 range)
{
	mapGenerator->DoBombAt(x, y, range);
}

void ABombermanTestGameModeBase::PlayerKilled()
{
	alivePlayers--;
}

// Called if the endgame conditions in the TICK function are satisfied.
void ABombermanTestGameModeBase::EndGame()
{
	gameActive = false;

	if (alivePlayers == 1)
	{
		TArray<AActor *> outActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABombermanPlayer::StaticClass(), outActors);

		// Get the surviving player, and pass it to the TriggerGameOver blueprint event.
		if (outActors.Num() > 0)
		{
			ABombermanPlayer *livingPlayer = Cast<ABombermanPlayer>(outActors[0]);
			TriggerGameOver(alivePlayers, livingPlayer);
		}
		else
		{
			// This should technically never be called, but we handle just in case no players were found.
			TriggerGameOver(0, nullptr);  // DRAW - both players dead.
		}

	}
	else
	{
		TriggerGameOver(alivePlayers, nullptr); // DRAW - Time ran out, or both players dead.
	}

}

void ABombermanTestGameModeBase::TriggerGameOver_Implementation(int32 numAlivePlayers, ABombermanPlayer *winner) const
{
	// Must also manually do the reverse of the second local player creation. Remove the player from the game
	// so that on restart, we don't have conflicting player counts.
	APlayerController * player = UGameplayStatics::GetPlayerController(GetWorld(), 1);
	UGameplayStatics::RemovePlayer(player, false);
}



