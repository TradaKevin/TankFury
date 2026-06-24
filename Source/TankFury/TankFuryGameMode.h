// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Tank.h"

#include "TankFuryGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TANKFURY_API ATankFuryGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	float GameOverDelay = 3.0f;

	UPROPERTY(EditAnywhere)
	int32 CountdownDelay = 3;

	int32 CountdownSeconds;

	FTimerHandle CountdownTimerHandle;

	bool IsVictory = false;

	ATank* Tank;
	int32 TowerCount;

	void ActorDied(AActor* DeadActor);

	void OnGameOverTimerTimeout();

	void OnCountdownTimerTimeout();
};
