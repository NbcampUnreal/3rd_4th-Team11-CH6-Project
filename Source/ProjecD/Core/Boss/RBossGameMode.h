// RBossGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "Core/Base/RGameModeBase.h"
#include "RBossGameMode.generated.h"

UCLASS()
class PROJECD_API ARBossGameMode : public ARGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	//보스 처치시 호출
	void HandleBossDefeated();
};
