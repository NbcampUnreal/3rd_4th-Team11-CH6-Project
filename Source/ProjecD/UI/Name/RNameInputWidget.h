#pragma once

#include "CoreMinimal.h"
#include "UI/Base/RBaseOutGameWidget.h"
#include "RNameInputWidget.generated.h"

class UEditableTextBox;
class UTextBlock;
class UButton;
class URGameInstance;
class UROutGameCharacterDataSubsystem;

/**
 * 닉네임 입력 UI 위젯
 * 캐릭터 생성 마지막 단계- 닉네임 입력 및 유효성 검사
 */
UCLASS()
class PROJECD_API URNameInputWidget : public URBaseOutGameWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
//----UI 요소 바인딩
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* NameInputBox;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ErrorMessageText; // 닉네임 규칙 위반시 나오는 에러메세지

	UPROPERTY(meta=(BindWidget))
	UTextBlock* RuleText; // 닉네임 규칙 안내하는 작은 글씨 관련

	UPROPERTY(meta=(BindWidget))
	UButton* CreateButton;

	UPROPERTY(meta=(BindWidget))
	UButton* CancelButton;

//------텍스트 변경 이벤트
	UFUNCTION()
	void OnNameTextChanged(const FText& Text);
	
//------버튼 클릭 이벤트
	UFUNCTION()
	void OnCreateClicked();

	UFUNCTION()
	void OnCancelClicked();
	
//------헬퍼 함수
	bool ValidateAndShowError(const FString& Name);
	URGameInstance* GetGameInstance() const;

	
};
