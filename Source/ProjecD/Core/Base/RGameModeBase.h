// RGameModeBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RGameModeBase.generated.h"

UCLASS()
class PROJECD_API ARGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARGameModeBase();

protected:
	virtual void BeginPlay() override;
	//공통 게임 시작 처리
	virtual void HandleGameStart();
	//공통 게임 종료 처리
	virtual void HandleGameEnd(bool bIsVictory);
};
