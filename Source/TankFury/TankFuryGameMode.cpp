// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFuryGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "TankFuryGameInstance.h"

void ATankFuryGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();
	UE_LOG(LogTemp, Display, TEXT("TowerCount: %d"), TowerCount);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		Tank = Cast<ATank>(PlayerPawn);
		if (!Tank)
		{
			UE_LOG(LogTemp, Display, TEXT("Faild to Find Tank Actor!"));
		}
	}
	int32 LoopIndex = 0;
	while (LoopIndex < TowerCount)
	{
		AActor* TowerActor = Towers[LoopIndex];
		if (TowerActor)
		{
			ATower* Tower = Cast<ATower>(TowerActor);
			if (Tower && Tank)
			{
				Tower->Tank = Tank;
				//UE_LOG(LogTemp, Display, TEXT("%s: Tank Variable Set!"), *Tower->GetActorNameOrLabel());
			}
		}

		LoopIndex++;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);

		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->AddToPlayerScreen();
			ScreenMessageWidget->SetMessageText("Get Ready!");
		}
	}

	

	CountdownSeconds = CountdownDelay;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ATankFuryGameMode::OnCountdownTimerTimeout, 1.0f, true);
}


void ATankFuryGameMode::OnCountdownTimerTimeout()
{
	CountdownSeconds--;

	if (CountdownSeconds > 0)
	{
		ScreenMessageWidget->SetMessageText(FString::FromInt(CountdownSeconds));
	}
	else if (CountdownSeconds == 0)
	{
		ScreenMessageWidget->SetMessageText("Start!");
		Tank->SetPlayerEnabled(true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		UE_LOG(LogTemp, Display, TEXT("Clear Timer!"));
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ATankFuryGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;
	IsVictory = false;

	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();

		IsGameOver = true;
	}
	else
	{
		ATower* DeadTower = Cast<ATower>(DeadActor);

		if (DeadTower)
		{
			DeadTower->HandleDestruction();

			TowerCount--;

			if (TowerCount <= 0)
			{
				IsGameOver = true;
				IsVictory = true;

			}
		}
	}

	if (IsGameOver)
	{
		FString GameOverString = IsVictory ? "Victory!" : "Defeat!";

		ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);

		ScreenMessageWidget->SetMessageText(GameOverString);

		FTimerHandle GameOverTimerHandle;

		GetWorldTimerManager().SetTimer(
			GameOverTimerHandle,
			this,
			&ATankFuryGameMode::OnGameOverTimerTimeout,
			GameOverDelay,
			false
		);
	}
}

void ATankFuryGameMode::OnGameOverTimerTimeout()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UTankFuryGameInstance* TankFuryGameInstance = Cast<UTankFuryGameInstance>(GameInstance);

		if (TankFuryGameInstance)
		{
			if (IsVictory)
			{
				TankFuryGameInstance->LoadNextLevel();
			}
			else
			{
				TankFuryGameInstance->RestartCurrentLevel();
			}
		}
	}
}

