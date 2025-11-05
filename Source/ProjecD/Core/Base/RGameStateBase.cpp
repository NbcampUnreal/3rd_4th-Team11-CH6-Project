// RGaemStateBase.cpp

#include "Core/Base/RGameStateBase.h"
#include "Net/UnrealNetwork.h"

void ARGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 서버 값 - 클라이언트 자동 동기화
	DOREPLIFETIME(ARGameStateBase, CurrentPhase);
	DOREPLIFETIME(ARGameStateBase, bIsGameEnded);
}

