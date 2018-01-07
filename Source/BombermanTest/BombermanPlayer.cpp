// Fill out your copyright notice in the Description page of Project Settings.

#include "BombermanPlayer.h"
#include "Engine.h"

int32 ABombermanPlayer::pCount = 0;

// Sets default values
ABombermanPlayer::ABombermanPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bombsOut = 0;
	maxBombs = 1;
	walkSpeed = 500.0f;

}

// Called when the game starts or when spawned
void ABombermanPlayer::BeginPlay()
{

	playerNum = pCount % 2;
	pCount++;

	Super::BeginPlay();
	
}

// Called every frame
void ABombermanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MovementInput.IsZero())
	{
		UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(GetCharacterMovement());
		MovementInput = MovementInput.GetSafeNormal() * walkSpeed;
		FVector move = FVector(MovementInput.X, MovementInput.Y, 0);
		MoveComp->AddInputVector(move);
	}

}

// Called to bind functionality to input
void ABombermanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UE_LOG(LogTemp, Warning, TEXT("Setup new player %d"), playerNum);

	if (playerNum == 0)
	{
		InputComponent->BindAxis("MoveForward", this, &ABombermanPlayer::MoveForward);
		InputComponent->BindAxis("MoveRight", this, &ABombermanPlayer::MoveRight);
		InputComponent->BindAction("PlantBomb", IE_Pressed, this, &ABombermanPlayer::PlantBomb);
	}
	else if (playerNum == 1)
	{
		InputComponent->BindAxis("MoveForward2", this, &ABombermanPlayer::MoveForward);
		InputComponent->BindAxis("MoveRight2", this, &ABombermanPlayer::MoveRight);
		InputComponent->BindAction("PlantBomb2", IE_Pressed, this, &ABombermanPlayer::PlantBomb);
	}

}

void ABombermanPlayer::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void ABombermanPlayer::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void ABombermanPlayer::PlantBomb()
{

}

