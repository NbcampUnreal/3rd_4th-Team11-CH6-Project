// RDungeonGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "Core/Base/RGameModeBase.h"
#include "RDungeonGameMode.generated.h"

UCLASS()
class PROJECD_API ARDungeonGameMode : public ARGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	//웨이브 시작
	void StartWave(int32 WaveIndex);
};
