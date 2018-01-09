// Fill out your copyright notice in the Description page of Project Settings.

#include "BombermanPlayer.h"
#include "Engine.h"
#include "Bomb.h"
#include "BombermanTestGameModeBase.h"

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

	if (remoteControlled)
	{
		remoteControlTimeLeft -= DeltaTime;
		if (remoteControlTimeLeft <= 0)
			remoteControlled = false;
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

void ABombermanPlayer::RegisterBombExploded()
{
	bombsOut--;
}

void ABombermanPlayer::Die()
{
	GetWorld()->DestroyActor(this);
	ABombermanTestGameModeBase::instance->PlayerKilled();
}

void ABombermanPlayer::PlantBomb()
{
	if (!remoteControlled)
	{
		if (bombsOut < maxBombs)
		{

			if (BombBlueprint)
			{
				bombsOut++;
				FVector pos = GetActorLocation();
				int32 x = (pos.X + 50.0f) / 100;
				int32 y = (pos.Y + 50.0f) / 100;

				pos.X = x * 100;
				pos.Y = y * 100;
				ABomb* b = GetWorld()->SpawnActor<ABomb>(BombBlueprint, pos, FRotator::ZeroRotator);
				b->Init(this, 2.0f, bombRange);
				b->OnExplodeDelegate.AddUObject(this, &ABombermanPlayer::RegisterBombExploded); // Register the bomb to notify player when it explodes
			}
		}
	}
	else
	{
		// If remote controlled...
		if (bombsOut == 0)
		{
			// and no bombs out, then place a bomb.
			if (BombBlueprint)
			{
				bombsOut++;
				FVector pos = GetActorLocation();
				int32 x = (pos.X + 50.0f) / 100;
				int32 y = (pos.Y + 50.0f) / 100;

				pos.X = x * 100;
				pos.Y = y * 100;
				ABomb* b = GetWorld()->SpawnActor<ABomb>(BombBlueprint, pos, FRotator::ZeroRotator);
				b->Init(this, 10.0f, bombRange); // Set time to 10 seconds.
				b->OnExplodeDelegate.AddUObject(this, &ABombermanPlayer::RegisterBombExploded); // Register the bomb to notify player when it explodes
				activeRemoteBomb = b;
			}
		}
		else
		{
			// otherwise, detonate the bomb that the player put out.
			activeRemoteBomb->Explode();
		}
	}
}

