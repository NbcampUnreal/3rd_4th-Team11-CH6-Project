// RDungeonGameMode.cpp

#include "Core/Dungeon/RDungeonGameMode.h"

void ARDungeonGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("[RDungeonGameMode] 던전 시작"));
	StartWave(1);//1웨이브 부터 시작
}

void ARDungeonGameMode::StartWave(int32 WaveIndex)
{
	UE_LOG(LogTemp, Log, TEXT("[RDungeonGameMode] 웨이브 %d 시작!"), WaveIndex);
}