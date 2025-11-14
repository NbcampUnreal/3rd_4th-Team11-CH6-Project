#pragma once

#include "CoreMinimal.h"
#include "UI/Base/RBaseOutGameWidget.h"
#include "Core/RGameInstance.h"
#include "RClassDetailWidget.generated.h"

class UButton;
class UTextBlock;

/**
 * 선택한 직업의 상세 정보 화면
 * -직업명, 보유 설명 표시
 * -확인(Y)/ 취소(N)버튼
 */
UCLASS()
class PROJECD_API URClassDetailWidget : public URBaseOutGameWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetClassInfo(ECharacterClassType ClassType);

private:
	//왼쪽 카드 UI (버튼들)
	UPROPERTY(meta=(BindWidget))
	UButton* ClassInfoButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* BasicAttackButton;

	//오른쪽 카드 UI(설명 카드)
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DetailTitleText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* DetailContentText;

	//하단 버튼 (Yes or NO)
	UPROPERTY(meta=(BindWidget))
	UButton* ConfirmButton;

	UPROPERTY(meta=(BindWidget))
	UButton* CancelButton;

//======버튼 이벤트들==========
	//왼쪽 버튼 이벤트
	UFUNCTION()
	void OnClassInfoButtonClicked();

	UFUNCTION()
	void OnBasicAttackButtonClicked();

	//하단 버튼 이벤트
	UFUNCTION()
	void OnConfirmClicked();

	UFUNCTION()
	void OnCancelClicked();

//========헬퍼 함수들============
	URGameInstance* GetGameInstance() const;

	//각 섹션별 정보 가져오기
	void GetClassDescription(FString& OutTitle,FString& OutContent) const;
	void GetBasicAttackDescription(FString& OutTitle,FString& OutContent) const;

	// 현재 선택된 직업
	ECharacterClassType CurrentClassType;
	
	
};
