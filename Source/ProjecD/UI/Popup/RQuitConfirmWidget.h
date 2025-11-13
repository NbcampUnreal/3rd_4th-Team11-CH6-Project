#pragma once

#include "CoreMinimal.h"
#include "UI/Base/RBaseOutGameWidget.h"
#include "RQuitConfirmWidget.generated.h"

class UButton;
class UTextBlock;

/**
 * 게임 종료 확인 팝업 위젯
 */
UCLASS()
class PROJECD_API URQuitConfirmWidget : public URBaseOutGameWidget
{
	GENERATED_BODY()

protected:
	// 초기화
	virtual void NativeConstruct() override;

	//팝업 설정
	virtual void InitializeUI() override;
	virtual void CleanupUI() override;

	//ShowUI() 오버라이드
	virtual void ShowUI() override;

private:
	// UI요소
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> YesButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> NoButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MessageText;

	// 버튼 클릭 이벤트
	UFUNCTION()
	void OnYesButtonClicked();

	UFUNCTION()
	void OnNoButtonClicked();

	
};
