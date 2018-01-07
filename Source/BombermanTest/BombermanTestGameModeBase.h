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

	UPROPERTY(EditAnywhere, Category = SpawningBP)
		TSubclassOf<AMapGenerator> MapGeneratorActor;
	
	static ABombermanTestGameModeBase *instance;

	UPROPERTY(BlueprintReadWrite)
		float timeLeft = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float roundTime = 180.0f;

	UFUNCTION(BlueprintCallable)
		void StartGame();

	void ExplodeBombAt(int32 x, int32 y, ABombermanPlayer *player, int32 range);
	
private:
	int32 alivePlayers = 0;
	bool gameActive = false;
	AMapGenerator *mapGenerator = nullptr;

};
