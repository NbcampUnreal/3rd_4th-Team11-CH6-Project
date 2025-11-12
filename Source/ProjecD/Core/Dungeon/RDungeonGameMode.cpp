// RDungeonGameMode.cpp

#include "RDungeonGameMode.h"
#include "RWaveManager.h"
#include "LobbyPotalActor.h"
#include "Kismet/GameplayStatics.h"

ARDungeonGameMode::ARDungeonGameMode()
{
	CurrentWave = 0;
}


void ARDungeonGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Log, TEXT("[RDungeonGameMode] 던전 시작"));
	
	WaveManager = GetWorld()->SpawnActor<ARWaveManager>();

	StartNextWave();
}

void ARDungeonGameMode::StartNextWave()
{
	if (!WaveManager) return;

	CurrentWave++;
	
	if (CurrentWave > MaxWaveCount)
	{
		SpawnPortal();
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[RDungeonGameMode] Wave %d 시작!"), CurrentWave);

	WaveManager->StartWave(CurrentWave, this);
}

void ARDungeonGameMode::OnWaveCleared()
{
	UE_LOG(LogTemp, Log, TEXT("[RDungeonGameMode] Wave %d 클리어!"), CurrentWave);

	StartNextWave();
}

void ARDungeonGameMode::SpawnPortal()
{
	UE_LOG(LogTemp, Log, TEXT("[RDungeonGameMode] 모든 웨이브 클리어! 포탈 생성!"));

	FVector PortalLocation(0, 0, 100);

	GetWorld()->SpawnActor<ALobbyPotalActor>(
		ALobbyPotalActor::StaticClass(),
		PortalLocation,
		FRotator::ZeroRotator
		);
}