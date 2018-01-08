// Fill out your copyright notice in the Description page of Project Settings.

#include "BombermanSmartCamera.h"
#include "BombermanPlayer.h"
#include "Bomb.h"
#include "Powerup.h"
#include "Engine.h"

// Called when the game starts or when spawned
void ABombermanSmartCamera::BeginPlay()
{
	Super::BeginPlay();
	desiredPosition = GetActorLocation();
}

void ABombermanSmartCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor *> found1;
	TArray<AActor *> found2;
	TArray<AActor *> found3;

	int32 cnt = 0;

	FVector forward = GetActorForwardVector();
	FVector center = FVector(0, 0, 0);

	TArray<FVector2D> pointsToBound;

	// Get all players, add to points array.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABombermanPlayer::StaticClass(), found1);
	for (AActor * actor : found1)
	{
		FVector addPoint = actor->GetActorLocation();
		pointsToBound.Add(FVector2D(addPoint.X, addPoint.Y));
	}

	// Get all bombs, add to points array
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABomb::StaticClass(), found2);
	for (AActor * actor : found2)
	{
		FVector addPoint = actor->GetActorLocation();
		pointsToBound.Add(FVector2D(addPoint.X, addPoint.Y));
	}

	// Get all powerups, add to points array
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APowerup::StaticClass(), found3);
	for (AActor * actor : found3)
	{
		FVector addPoint = actor->GetActorLocation();
		pointsToBound.Add(FVector2D(addPoint.X, addPoint.Y));
	}

	if (pointsToBound.Num() > 1)
	{

		FBox2D box2D = FBox2D(pointsToBound);

		FVector2D center2d = box2D.GetCenter();

		float dist = FMath::Max(box2D.GetSize().X, box2D.GetSize().Y);
		dist = FVector2D::Distance(box2D.Min, box2D.Max);

		dist = FMath::Clamp(dist, minZoom, maxZoom);
		center.X = center2d.X;
		center.Y = center2d.Y;
		center.Z = 0;

		desiredPosition = center + forward * -(dist * scaleFactor);

	}
	else if (pointsToBound.Num() == 1)
	{
		FVector2D center2d = pointsToBound[0];
		center.X = center2d.X;
		center.Y = center2d.Y;
		center.Z = 0;

		desiredPosition = center + forward * -(minZoom * scaleFactor);
	}

	SetActorLocation(FMath::Lerp(GetActorLocation(), desiredPosition, lerpSpeed * DeltaTime));

}
