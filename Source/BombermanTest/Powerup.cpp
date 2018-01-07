// Fill out your copyright notice in the Description page of Project Settings.

#include "Powerup.h"


// Sets default values
APowerup::APowerup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APowerup::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APowerup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator rotator = FRotator(0, DeltaTime * 100.0f, 0);
	AddActorWorldRotation(rotator);
}

void APowerup::ExecutePickup_Implementation(ABombermanPlayer * who) const
{
	return;
}

