// RGameStateBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RGameStateBase.generated.h"

UCLASS()
class PROJECD_API ARGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	// 현제 게임 단계 (0=로비 1=던전 2=보스 등)
	UPROPERTY(Replicated)
	int32 CurrentPhase = 0;

	UPROPERTY(Replicated)
	bool bIsGameEnded = false;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
