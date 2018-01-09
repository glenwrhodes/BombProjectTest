// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MapGenerator.h"
#include "BombermanPlayer.h"
#include "BombermanTestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANTEST_API ABombermanTestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	/// The Map Generator actor to spawn.
	UPROPERTY(EditAnywhere, Category = SpawningBP)
		TSubclassOf<AMapGenerator> MapGeneratorActor;

	/// The AI player to spawn.
	UPROPERTY(EditAnywhere, Category = SpawningBP)
		TSubclassOf<ABombermanPlayer> AIPlayer;
	
	/// Singleton instance for quick access.
	static ABombermanTestGameModeBase *instance;

	/// Time left in the round.
	UPROPERTY(BlueprintReadWrite)
		float timeLeft = 180.0f;

	/// Round time total.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float roundTime = 180.0f;

	/// Whether or not to use AI in this round.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool useAI = false;

	/// Starts the game. Initializes variables.
	UFUNCTION(BlueprintCallable)
		void StartGame();

	/// Explodes a bomb at position X and Y - communicates with the MapGenerator
	void ExplodeBombAt(int32 x, int32 y, ABombermanPlayer *player, int32 range);

	/// Called when players are killed.  Needs to be managed through game mode so that when players die in the same explosion, there's no time discrepency
	void PlayerKilled();

	/// Dispatches the end game event under TriggerGameOver
	UFUNCTION(BlueprintCallable)
		void EndGame();

	/// UE4 Blueprint event to handle round end.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Bomberman")
		void TriggerGameOver(int32 numAlivePlayers, ABombermanPlayer *winner) const;
	
private:
	int32 alivePlayers = 0;
	bool gameActive = false;
	AMapGenerator *mapGenerator = nullptr;

};
