// RWaveManager.cpp


#include "Core/Dungeon/RWaveManager.h"
#include "Core/Dungeon/RDungeonGameMode.h"
#include "Engine/World.h"
#include "TimerManager.h"

void ARWaveManager::StartWave(int32 WaveIndex, ARDungeonGameMode* GameModeRef)
{
	DungeonGM = GameModeRef;

	if (WaveIndex == 25)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Wave %d] 던전 보스 웨이브 시작!"), WaveIndex);
		SpawnFinalBoss();
		return;
	}

	if (WaveIndex % 5 == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Wave %d] 에픽 몬스터 웨이브 시작!"), WaveIndex);
		SpawnEpicWave(WaveIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Wave %d] 일반 웨이브 시작!"), WaveIndex);
		SpawnNormalWave(WaveIndex);
	}
}

void ARWaveManager::SpawnNormalWave(int32 WaveIndex)
{
	int32 MonsterCount = 5 + WaveIndex * 2;

	UE_LOG(LogTemp, Warning, TEXT(" -> 임시 일반 몬스터 %d마리 생성"), MonsterCount);

	//임시 액터 스폰
	for (int32 i = 0; i < MonsterCount; ++i)
	{
		FVector Loc(i * 150, FMath::RandRange(-300, 300), 100);
		GetWorld()->SpawnActor<AActor>(
			AActor::StaticClass(),
			Loc,
			FRotator::ZeroRotator
			);
	}

	CompleteWaveWithDelay(2.f); //2초뒤 웨이브 클리어 처리
}

void ARWaveManager::SpawnEpicWave(int32 WaveIndex)
{
	UE_LOG(LogTemp, Warning, TEXT(" -> 에픽 몬스터 1마리 + 일반 몬스터 소수 추가"));

	// 임시 액터 소환
	for (int32 i = 0; i < 10; ++i)
	{
		FVector Loc(i * 200, FMath::RandRange(-400, 400), 100);
		GetWorld()->SpawnActor<AActor>(
			AActor::StaticClass(),
			Loc,
			FRotator::ZeroRotator
			);
	}

	CompleteWaveWithDelay(3.f);
}

void ARWaveManager::SpawnFinalBoss()
{
	UE_LOG(LogTemp, Warning, TEXT(" -> 던전 보스 1마리 생성(임시 액터)"));

	// 임시 액터
	GetWorld()->SpawnActor<AActor>(
			AActor::StaticClass(),
			FVector(0, 0, 100),
			FRotator::ZeroRotator
			);

	CompleteWaveWithDelay(4.f);
}

void ARWaveManager::CompleteWaveWithDelay(float DelaySeconds)
{
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle,[this]()
	{
		UE_LOG(LogTemp, Warning, TEXT(" -> 웨이브 클리어 처리 완료!"));
		if (DungeonGM)
			DungeonGM->OnWaveCleared();
	}, DelaySeconds, false);
}