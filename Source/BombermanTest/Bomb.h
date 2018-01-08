// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

DECLARE_MULTICAST_DELEGATE(FExplodeDelegate);

class ABombermanPlayer; // Forward Declaration

UCLASS()
class BOMBERMANTEST_API ABomb : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ABombermanPlayer * myOwner = nullptr;
	float detonationTime = 3.0f;

	FExplodeDelegate OnExplodeDelegate; // Called by the bomb when it explodes.

	/* Initializes the bomb, declaring its player owner, detonation time and range
	*/
	void Init(ABombermanPlayer *owner, float detTime, int32 _range);

	UFUNCTION(BlueprintCallable)
		void Explode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool exploded = false;

private:
	float age = 0.0f;
	int32 range = 1;


};
