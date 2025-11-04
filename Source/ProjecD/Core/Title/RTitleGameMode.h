// RTitleGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "Core/Base/RGameModeBase.h"
#include "RTitleGameMode.generated.h"

UCLASS()
class PROJECD_API ARTitleGameMode : public ARGameModeBase
{
	GENERATED_BODY()

protected:
	// 타이틀 맵 시작 될때 호출
	virtual void BeginPlay() override;
};
