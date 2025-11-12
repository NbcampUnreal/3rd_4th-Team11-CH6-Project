//RWaveManager.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RWaveManager.generated.h"

class ARDungeonGameMode;

UCLASS()
class PROJECD_API ARWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	void StartWave(int32 WaveIndex, ARDungeonGameMode* GameModeRef);

protected:
	UPROPERTY()
	TObjectPtr<ARDungeonGameMode> DungeonGM;

	void SpawnNormalWave(int32 WaveIndex);
	void SpawnEpicWave(int32 WaveIndex);
	void SpawnFinalBoss();

	//현제 몬스터가 없는 관계로 웨이브 처리
	void CompleteWaveWithDelay(float DelaySeconds);


};
