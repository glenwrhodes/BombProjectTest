// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerator.h"
#include "Classes/AI/Navigation/NavigationSystem.h"
#include "Engine.h"

// Sets default values
AMapGenerator::AMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapGenerator::GenerateMapData()
{

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			mapData.Add((uint8)TileType::grass);
		}
	}

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (i == 0 || j == 0 || i == width - 1 || j == height - 1)
				mapData[(i * height) + j] = (uint8)TileType::invincible;

			if (i % 2 == 0 && j % 2 == 0)
				mapData[(i * height) + j] = (uint8)TileType::invincible;

		}
	}

	for (int i = 2; i < width - 1; i += 2)
	{
		for (int j = 2; j < height - 1; j += 2)
		{

			float rndir = FMath::FRand();

			if (rndir < 0.25f)
				mapData[((i - 1) * height) + j] = (uint8)TileType::breakable;
			if (rndir >= 0.25f && rndir < 0.5f)
				mapData[((i + 1) * height) + j] = (uint8)TileType::breakable;
			if (rndir >= 0.5f && rndir < 0.75f)
				mapData[(i * height) + (j + 1)] = (uint8)TileType::breakable;
			if (rndir >= 0.75f)
				mapData[(i * height) + (j - 1)] = (uint8)TileType::breakable;

		}
	}

	mapData[2 * height + 1] = (uint8)TileType::grass;
	mapData[1 * height + 2] = (uint8)TileType::grass;

}

void AMapGenerator::GenerateMapGeometry()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.bDeferConstruction = false;
	FRotator rot = FRotator(0, 0, 0);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			TileType b = (TileType)mapData[(i * height) + j];

			FVector location = FVector((float)i * scale, (float)j * scale, 0);
			FVector max = FVector((float)i * scale + scale, (float)j * scale + scale, scale);

			AMapTilePiece *NewTile = nullptr;

			switch (b)
			{
			case TileType::grass:
				NewTile = GetWorld()->SpawnActor<AMapTilePiece>(BPFloor, location, FRotator::ZeroRotator, SpawnInfo);
				break;
			case TileType::invincible:
				NewTile = GetWorld()->SpawnActor<AMapTilePiece>(BPInvinciblePiece, location, FRotator::ZeroRotator, SpawnInfo);
				break;
			case TileType::breakable:
				NewTile = GetWorld()->SpawnActor<AMapTilePiece>(BPWallBlock, location, FRotator::ZeroRotator, SpawnInfo);
				break;
			default:
				break;
			}

			mapPieces.Add(NewTile);

			GetWorld()->GetNavigationSystem()->AddDirtyArea(FBox(location, max), (1 << 0) | (1 << 1) || (1 << 2));

		}
	}
}

