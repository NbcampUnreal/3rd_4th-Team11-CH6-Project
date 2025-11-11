#pragma once

#include "CoreMinimal.h"
#include "UI/Base/RBaseOutGameWidget.h"
#include "Core/RGameInstance.h"
#include "RClassSelectWidget.generated.h"

class UButton;
class UTextBlock;
class UBorder;

/**
 *직업(클래스) 선택 UI 위젯
 * 클래스 고정배치- 나이트, 아처, 메이지순서로 좌에서 우
 * 이미 생성한 직업은 고를수 없게 비활성화
 */
UCLASS()
class PROJECD_API URClassSelectWidget : public URBaseOutGameWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	// 직업 버튼 갱신
	UFUNCTION(BlueprintCallable)
	void RefreshClassButtons(); 

private:
//=================UI 요소 ==================//
	// 나이트, 왼쪽, 고정 
	UPROPERTY(meta=(BindWidget)) UButton* KnightButton;
	UPROPERTY(meta=(BindWidget)) UTextBlock* KnightNameText;
	UPROPERTY(meta=(BindWidget)) UTextBlock* KnightStatusText; // 나이트 선택 또는 생성됨
	UPROPERTY(meta=(BindWidget)) UBorder* KnightBorder;  // 테두리 슬롯 관련
	
	// 아처, 가운데 ,고정
	UPROPERTY(meta=(BindWidget)) UButton* ArcherButton;
	UPROPERTY(meta=(BindWidget)) UTextBlock* ArcherNameText;
	UPROPERTY(meta=(BindWidget)) UTextBlock* ArcherStatusText; // 아처 선택 또는 생성됨
	UPROPERTY(meta=(BindWidget)) UBorder* ArcherBorder;

	//메이지, 오른쪽, 고정
	UPROPERTY(meta=(BindWidget)) UButton* MageButton;
	UPROPERTY(meta=(BindWidget)) UTextBlock* MageNameText;
	UPROPERTY(meta=(BindWidget)) UTextBlock* MageStatusText; // 마법사 선택 또는 생성됨
	UPROPERTY(meta=(BindWidget)) UBorder* MageBorder;

	//버튼 클릭 이벤트
	UFUNCTION() void OnKnightButtonClicked();
	UFUNCTION() void OnArcherButtonClicked();
	UFUNCTION() void OnMageButtonClicked();

//======헬퍼 함수들============

	//직업 선택 공통 처리
	void HandleClassSelection(ECharacterClassType SelectedClass);

	//특정 직업 버튼 상태 업데이트
	void UpdateClassButton(UButton* Button,UTextBlock* StatusText,ECharacterClassType Class);

	//특정 직업이 이미 생성되었는지 확인
	bool IsClassAlreadyCreated(ECharacterClassType Class) const;

	//게임 인스턴스 가져오기
	URGameInstance* GetGameInstance() const;
	
};
