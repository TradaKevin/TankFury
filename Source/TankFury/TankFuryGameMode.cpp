// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFuryGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Tower.h"

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
}

void ATankFuryGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		UE_LOG(LogTemp, Display, TEXT("Tank Died, Defeat!"));
	}
	else
	{
		ATower* DeadTower = Cast<ATower>(DeadActor);

		if (DeadTower)
		{
			UE_LOG(LogTemp, Display, TEXT("Tower Died!"));

			DeadTower->Destroy();

			TowerCount--;

			if (TowerCount <= 0)
			{
				UE_LOG(LogTemp, Display, TEXT("All Towers are Died, Taste  the victory (for first time you looser)!"));

			}
		}
	}
}
