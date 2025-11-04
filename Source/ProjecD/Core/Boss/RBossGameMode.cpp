// RBossGameMode.cpp

#include "Core/Boss/RBossGameMode.h"

void ARBossGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("[RBossGameMode] 보스전 시작"));
}

void ARBossGameMode::HandleBossDefeated()
{
	UE_LOG(LogTemp, Log, TEXT("[RBossGameMode] 보스 처치 - 리더 보드 기록 예정"));
}
