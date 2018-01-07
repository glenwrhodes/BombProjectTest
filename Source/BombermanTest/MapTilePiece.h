// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Powerup.h"
#include "MapTilePiece.generated.h"

UCLASS()
class BOMBERMANTEST_API AMapTilePiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapTilePiece();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = BomberMan)
		bool bIsDestructible = false;

	UPROPERTY(EditAnywhere, Category = BomberMan)
		bool bIsBlocking = false;

	UPROPERTY(EditAnywhere, Category = BomberMan)
		float dropChance = 0.3f;

	UPROPERTY(EditAnywhere, Category = BomberMan)
		TArray<TSubclassOf<APowerup>> droppablePickups;
	
};
