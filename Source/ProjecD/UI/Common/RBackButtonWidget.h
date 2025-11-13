#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RBackButtonWidget.generated.h"

class UButton;

/**
 * outgame ui에서 쓰이는 공통 뒤로가기 위젯 버튼
 */
UCLASS()
class PROJECD_API URBackButtonWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta=(BindWidget))
	UButton* BackButtonMain;

	UFUNCTION()
	void OnBackButtonClicked();
	
};
