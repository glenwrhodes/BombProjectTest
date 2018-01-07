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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool remoteControlled = false;

	static int32 pCount;

	UPROPERTY(EditAnywhere, Category = Bomberman)
		TSubclassOf<class ABomb> BombBlueprint;

	UFUNCTION()
		void RegisterBombExploded();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float remoteControlTimeLeft = 0;

	UFUNCTION(BlueprintCallable)
		void Die();


private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	FVector2D MovementInput;

	void PlantBomb();

	class ABomb* activeRemoteBomb;
	
};
