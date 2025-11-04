// RGameModeBase.cpp

#include "Core/Base/RGameModeBase.h"

ARGameModeBase::ARGameModeBase()
{
	bStartPlayersAsSpectators = false;
}

void ARGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	HandleGameStart(); //공통 시작
}

void ARGameModeBase::HandleGameStart()
{
	UE_LOG(LogTemp, Log, TEXT("[RGameModeBase] 게임 시작"));
}

void ARGameModeBase::HandleGameEnd(bool bIsVictory)
{
	UE_LOG(LogTemp, Log, TEXT("[RGameModeBase] 게임종료 - 결과 : %s"),
		bIsVictory ? TEXT("승리") : TEXT("패배"));
}