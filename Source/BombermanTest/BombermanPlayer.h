// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BombermanPlayer.generated.h"

UCLASS()
class BOMBERMANTEST_API ABombermanPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABombermanPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// This player's player number.. 0, 1, 2, 3, etc.  In this test, only valid numbers are 0 and 1.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 playerNum = 0;

	/// Walk speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float walkSpeed = 100;

	/// How far this player's bombs explosions will reach along X and Y
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 bombRange = 1;

	/// The max concurrent bombs a player can have out on the map.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 maxBombs = 1;

	/// How many bombs are currently out on the map.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 bombsOut = 0;

	/// Does the player currently have a remote control powerup?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool remoteControlled = false;

	/// Player count - incremented to make every player get a different player number
	static int32 pCount;

	/// The bomb blueprint to place when a player plants a bomb.
	UPROPERTY(EditAnywhere, Category = Bomberman)
		TSubclassOf<class ABomb> BombBlueprint;

	/// Called by the bomb delegate when a bomb explodes
	UFUNCTION()
		void RegisterBombExploded();

	/// Number of seconds left before the remote control powerup expires.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float remoteControlTimeLeft = 0;

	/// Called by the explosion manager in mapgenerator when the player dies in an explosion.
	UFUNCTION(BlueprintCallable)
		void Die();

	/// Plants a bomb at the current grid coordinate.
	UFUNCTION(BlueprintCallable)
		void PlantBomb();

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	FVector2D MovementInput;

	class ABomb* activeRemoteBomb;
	
};
