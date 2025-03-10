// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerator.h"
#include "Classes/AI/Navigation/NavigationSystem.h"
#include "Bomb.h"
#include "BombermanPlayer.h"
#include "Powerup.h"
#include "Engine.h"

/// Sets default values
AMapGenerator::AMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

/// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

/// Called every frame
void AMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// Pre-generate the data that defines the map.
void AMapGenerator::GenerateMapData()
{

	// Initialize everything to grass.
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
			// Edge rows
			if (i == 0 || j == 0 || i == width - 1 || j == height - 1)
				mapData[(i * height) + j] = (uint8)TileType::invincible;

			// The even spacing of invincible tiles throughout
			if (i % 2 == 0 && j % 2 == 0)
				mapData[(i * height) + j] = (uint8)TileType::invincible;

		}
	}

	// For each interior invincible piece, randomly spawn one block above, below, or beside it.
	// This generates the random board.
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

	// Fix so that players won't get boxed in by two breakable pieces - which would force
	// suicide when detonating the bomb. Remove the pieces below and beside the player.

	// Player 1
	mapData[14 * height + 1] = (uint8)TileType::grass;
	mapData[15 * height + 2] = (uint8)TileType::grass;

	// Player 2
	mapData[14 * height + 15] = (uint8)TileType::grass;
	mapData[15 * height + 14] = (uint8)TileType::grass;

	// AI Player
	mapData[7 * height + 7] = (uint8)TileType::grass;
	mapData[7 * height + 8] = (uint8)TileType::grass;

}

/// Generate the actual geometry of the map. Spawn tiles based on what was generated in the map data.
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

			// Make the nagivation system regenerate this tile for future AI addition.
			GetWorld()->GetNavigationSystem()->AddDirtyArea(FBox(location, max), (1 << 0) | (1 << 1) || (1 << 2));

		}
	}
}

/// Called on each block to see if we should proceed beyond this block, or terminate the outward search.
/// @param indx - The tile index being checked
bool AMapGenerator::DidDestroyBlock(int32 indx)
{
	bool shouldProceed = true; // Should we proceed with the check

	if (indx >= 0)
	{
		AMapTilePiece* blockPiece = mapPieces[indx];

		if (blockPiece)
		{
			if (blockPiece->bIsDestructible)
			{

				// Replace the destructible tile with a grass piece
				AMapTilePiece* NewTile = GetWorld()->SpawnActor<AMapTilePiece>(BPFloor, blockPiece->GetActorLocation(), FRotator::ZeroRotator);
				GetWorld()->DestroyActor(blockPiece); // Flag block for deletion.
				mapPieces[indx] = NewTile;
				mapData[indx] = (uint8)TileType::grass;

				// This is a breakable tile, which means the explosion will not propagate further after destroying the tile.
				shouldProceed = false; 

				CheckOverlappingGameElements(blockPiece->GetActorLocation());

				// This block was destroyed - should we spawn a pickup?
				if (FMath::FRand() <= blockPiece->dropChance)
				{
					if (blockPiece->droppablePickups.Num() > 0)
					{
						int32 s = FMath::RandRange(0, blockPiece->droppablePickups.Num() - 1);
						TSubclassOf<APowerup> p = blockPiece->droppablePickups[s];
						FVector spawnLocation = blockPiece->GetActorLocation();
						spawnLocation.Z = 54;
						GetWorld()->SpawnActor<APowerup>(p, spawnLocation, FRotator::ZeroRotator);
					}
				}

				GetWorld()->SpawnActor<AActor>(Explosion, blockPiece->GetActorLocation(), FRotator::ZeroRotator);

			}
			else
				shouldProceed = false;

			if (!blockPiece->bIsBlocking)
			{
				shouldProceed = true; // It's Grass, so don't destroy.
				GetWorld()->SpawnActor<AActor>(Explosion, blockPiece->GetActorLocation(), FRotator::ZeroRotator);
				CheckOverlappingGameElements(blockPiece->GetActorLocation());
			}

		}
	}

	return shouldProceed;
}

/// Check to see what, if anything we are overlapping at a position.
/// @param position - The vector position in worldspace we are checking.
void AMapGenerator::CheckOverlappingGameElements(FVector position)
{
	TArray<AActor*> outActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // All object types
	TArray<AActor*> ActorsToIgnore;

	// Overlap check a square area, which matches the size of the maze grid.
	bool result = UKismetSystemLibrary::BoxOverlapActors(GetWorld(), position, FVector(50, 50, 50), ObjectTypes, nullptr, ActorsToIgnore, outActors);

	if (result)
	{
		for (AActor* a : outActors)
		{
			UClass *type = a->GetClass();

			if (type->IsChildOf(ABombermanPlayer::StaticClass()))
			{
				// If the explosion is overlapping a player, kill them.
				ABombermanPlayer* p = Cast<ABombermanPlayer>(a);
				p->Die();
			}
			else if (type->IsChildOf(APowerup::StaticClass()))
			{
				// If the explosion is overlapping a powerup, destroy it.
				APowerup* p = Cast<APowerup>(a);
				GetWorld()->DestroyActor(p);
			}
			else if (type->IsChildOf(ABomb::StaticClass()))
			{
				// If the explosion is overlapping another bomb, trigger its explosion.
				ABomb* p = Cast<ABomb>(a);
				p->Explode();
			}
		}
	}
}

/// Begin the process of bombing a particular x and y coordinate on the grid.
/// @param x - X coordinate in the maze grid
/// @param y - Y coordinate in the maze grid
/// @param range - how far in each of the 4 directions to propagate the explosion.
void AMapGenerator::DoBombAt(int32 x, int32 y, int32 range)
{

	// Horizontal right
	for (int i = x + 1; i <= x + range; i++)
	{
		int indx = (i * height) + y;
		if (indx >= 0 && i >= 0 && i < width)
		{
			if (!DidDestroyBlock(indx))
				break;
		}
	}

	// Horizontal left - starts ON bomb position. This is the only direction that also
	// includes the bomb position - so, i = x
	for (int i = x; i >= x - range; i--)
	{
		int indx = (i * height) + y;
		if (indx >= 0 && i >= 0 && i < width)
		{
			if (!DidDestroyBlock(indx))
				break;
		}
	}

	// Vertical down
	for (int j = y + 1; j <= y + range; j++)
	{
		int indx = (x * height) + j;
		if (indx >= 0 && j >= 0 && j < height)
		{
			if (!DidDestroyBlock(indx))
				break;
		}
	}

	// Vertical up
	for (int j = y - 1; j >= y - range; j--)
	{
		int indx = (x * height) + j;
		if (indx >= 0 && j >= 0 && j < height)
		{
			if (!DidDestroyBlock(indx))
				break;
		}
	}
}


