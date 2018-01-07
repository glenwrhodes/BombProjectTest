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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 playerNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float walkSpeed = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 bombRange = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 maxBombs = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 bombsOut = 0;

	static int32 pCount;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	FVector2D MovementInput;

	void PlantBomb();

	
};
