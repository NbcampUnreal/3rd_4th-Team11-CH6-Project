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

	//플레이어 로그인 완료후 호출(PlayerController 생성 이후)
	virtual void PostLogin(APlayerController* NewPlayer) override;

	//UI팀 설정 (타이틀 화면 위젯 클래스
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="UI")
	TSubclassOf<class URBaseOutGameWidget> TitleWidgetClass;
};
