// Fill out your copyright notice in the Description page of Project Settings.

#include "Bomb.h"
#include "BombermanTestGameModeBase.h"

// Sets default values
ABomb::ABomb()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

/// Called when the game starts or when spawned
void ABomb::BeginPlay()
{
	Super::BeginPlay();
}

/// Called every frame
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	age += DeltaTime;
	if (age >= detonationTime && !exploded)
	{
		Explode();
	}
}

/// Tells the bomb to 'explode'. Talks to the game mode to propagate the explosion to the map generator.
/// and communicates via a delegate to all listening players.
void ABomb::Explode()
{
	if (exploded) return;
	exploded = true;

	FVector pos = GetActorLocation();
	int32 x = (pos.X + 50.0f) / 100;
	int32 y = (pos.Y + 50.0f) / 100;

	ABombermanTestGameModeBase::instance->ExplodeBombAt(x, y, myOwner, range);

	// Tell anyone who is listening that the bomb exploded.
	if (OnExplodeDelegate.IsBound())
	{
		OnExplodeDelegate.Broadcast();
		OnExplodeDelegate.Clear();
	}

	GetWorld()->DestroyActor(this);
	PrimaryActorTick.bCanEverTick = false;
}

/// Initialize the bomb - set owner, detonation time, and the explosion range.
void ABomb::Init(ABombermanPlayer * owner, float detTime, int32 _range)
{
	myOwner = owner;
	detonationTime = detTime;
	range = _range;
}

