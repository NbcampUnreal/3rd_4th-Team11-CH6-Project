#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RBaseOutGameWidget.generated.h"

class UButton;
class UROutGameUIManager;

/**
 *  모든 OutGame UI의 부모 클래스
 */
UCLASS()
class PROJECD_API URBaseOutGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	URBaseOutGameWidget(const FObjectInitializer& ObjectInitializer);


	//자식 위젯 override용
	virtual void InitializeUI();
	virtual void CleanupUI();

	
	//UI 표시 및 숨기기
	UFUNCTION(BlueprintCallable,Category="UI")
	virtual void ShowUI();

	UFUNCTION(BlueprintCallable,Category="UI")
	virtual void HideUI();

	//뒤로가기
	virtual bool CanGoBack() const { return true; }\

	//포커스 관련 설정
	virtual bool Initialize() override;

protected:
	// 초기화
	virtual void NativeConstruct() override;

	// 뒤도가기 키 입력 처리
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	//UI Manager를 가져오기 위한 Getter함수 추가
	UROutGameUIManager* GetUIManager() const;
	
};
