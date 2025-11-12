// RDungeonGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "Core/Base/RGameModeBase.h"
#include "RDungeonGameMode.generated.h"

class ARWaveManager;
class APortalActor;

UCLASS()
class PROJECD_API ARDungeonGameMode : public ARGameModeBase
{
	GENERATED_BODY()

public:
    ARDungeonGameMode();
    
	virtual void BeginPlay() override;
	
	//웨이브 시작
	void StartNextWave();

	// 웨이브 몬스터 처치 시 호출
	void OnWaveCleared();
	
	//최종 마을가는 포탈 생성
	void SpawnPortal();
	
protected:
	//웨이브 매니저
	UPROPERTY()
	TObjectPtr<ARWaveManager> WaveManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	int32 CurrentWave;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	int32 MaxWaveCount = 25;
	
};
