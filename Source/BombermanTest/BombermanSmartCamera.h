// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "BombermanSmartCamera.generated.h"

/**
*
*/
UCLASS()
class BOMBERMANTEST_API ABombermanSmartCamera : public ACameraActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		float scaleFactor = 1.0f;

	UPROPERTY(EditAnywhere)
		float lerpSpeed = 0.4f;

	UPROPERTY(EditAnywhere)
		float maxZoom = 2700.0f;

	UPROPERTY(EditAnywhere)
		float minZoom = 1500.0f;

private:
	FVector desiredPosition;

};
