#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RGameInstance.generated.h"

class UOutGameUIManager;

/**
 * 
 */
UCLASS()
class PROJECD_API URGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	URGameInstance();

	virtual void Init() override;

	//UI팀  관련 설정
	UFUNCTION(BlueprintCallable,Category="UI")
	UROutGameUIManager* GetUIManager() const { return UIManager; }

private:
	//UI팀 관련 설정
	UPROPERTY()
	UROutGameUIManager* UIManager;
	
};
