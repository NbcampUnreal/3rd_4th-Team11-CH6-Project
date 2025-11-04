#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RBaseOutGameWidget.generated.h"

/**
 *  모든 OutGame UI의 부모 클래스
 */
UCLASS()
class PROJECD_API URBaseOutGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	URBaseOutGameWidget(const FObjectInitializer& ObjectInitializer);

	//UI 표시 및 숨기기
	UFUNCTION(BlueprintCallable,Category="UI")
	virtual void ShowUI();

	UFUNCTION(BlueprintCallable,Category="UI")
	virtual void HideUI();

protected:
	// 초기화
	virtual void NativeConstruct() override;
	
	
};
