// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapTilePiece.h"
#include "MapGenerator.generated.h"

UENUM(BlueprintType)
enum class TileType : uint8
{
	grass = 0,
	invincible = 1,
	breakable = 2
};

UCLASS()
class BOMBERMANTEST_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AMapTilePiece> BPFloor;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AMapTilePiece> BPWallBlock;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AMapTilePiece> BPInvinciblePiece;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> Explosion;

	UPROPERTY(EditAnywhere)
		uint16 width = 32;

	UPROPERTY(EditAnywhere)
		uint16 height = 32;

	UPROPERTY(EditAnywhere)
		float scale = 100;

	/* Generates the random map data - fills in the mapData array, and populates it with 0, 1 or 2, where
	* 0 = grass
	* 1 = unbreakable tile
	* 2 = breakable tile
	*/
	void GenerateMapData();

	/* From the defined map data, spawn the geometry to build the level. */
	void GenerateMapGeometry();

	/* Applies a bomb to a specific map coordinate, with a specific range on X and Y.
	Responsible for destroying the blocks, and playing an explosion animation
	*/
	void DoBombAt(int32 x, int32 y, int32 range);
	void CheckOverlappingGameElements(FVector position);
	bool DidDestroyBlock(int32 indx);
	
private:
	TArray<TileType> tiles;
	TArray<uint8> mapData;
	TArray<AMapTilePiece*> mapPieces;
	
};
