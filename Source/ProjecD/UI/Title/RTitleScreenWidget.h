#pragma once

#include "CoreMinimal.h"
#include "UI/Base/RBaseOutGameWidget.h"
#include "RTitleScreenWidget.generated.h"

class UButton;
class UROutGameUIManager;

/**
 * 타이틀 화면 위젯
 * Play,OPTIONS,CREDITS,QUIT 버튼 제공
 */
UCLASS()
class PROJECD_API URTitleScreenWidget : public URBaseOutGameWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	// UI 컴포넌트- 블루프린트 바인딩용
	UPROPERTY(meta=(BindWidget))
	UButton* PlayButton;

	UPROPERTY(meta=(BindWidget))
	UButton* OptionsButton;

	UPROPERTY(meta=(BindWidget))
	UButton* CreditsButton;

	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton;

private:
	// 버튼 클릭 이벤트들
	UFUNCTION()
	void OnPlayClicked();

	UFUNCTION()
	void OnOptionsClicked();

	UFUNCTION()
	void OnCreditsClicked();
	
	UFUNCTION()
	void OnQuitButtonClicked();

public:
	virtual bool CanGoBack() const override {return false;} // 타이틀 화면은 Quit이 있으므로.
	
};
