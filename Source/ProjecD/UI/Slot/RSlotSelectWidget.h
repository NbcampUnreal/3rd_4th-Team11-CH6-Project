#pragma once

#include "CoreMinimal.h"
#include "UI/Base/RBaseOutGameWidget.h"
#include "Core/RGameInstance.h"
#include "RSlotSelectWidget.generated.h"

class UButton;
class UTextBlock;

/**
 * 캐릭터 생성하는 슬롯 위젯
 * 슬롯은 총 3개로 제한
 */
UCLASS()
class PROJECD_API URSlotSelectWidget : public URBaseOutGameWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	//슬롯 새로고침- 캐릭 생성 및 삭제 후 호출됨
	UFUNCTION(BlueprintCallable)
	void RefreshSlots();

private:
	//슬롯 버튼 클릭
	UFUNCTION()
	void OnSlot0Clicked();
	UFUNCTION()
	void OnSlot1Clicked();
	UFUNCTION()
	void OnSlot2Clicked();

	//슬롯 클릭 공통 처리
	UFUNCTION()
	void HandleSlotClick(int32 SlotIndex);

	//GameInstance 가져오기
	URGameInstance* GetGameInstance() const;

	//슬롯 UI 업데이트
	void UpdateSlotUI(int32 SlotIndex,UButton* Button,UTextBlock* NameText,UTextBlock* ClassText);

	// UI 바인딩
	UPROPERTY(meta=(BindWidget)) UButton* Slot0Button;
	UPROPERTY(meta=(BindWidget)) UTextBlock* Slot0Name;
	UPROPERTY(meta=(BindWidget)) UTextBlock* Slot0Class;

	UPROPERTY(meta=(BindWidget)) UButton* Slot1Button;
	UPROPERTY(meta=(BindWidget)) UTextBlock* Slot1Name;
	UPROPERTY(meta=(BindWidget)) UTextBlock* Slot1Class;

	UPROPERTY(meta=(BindWidget)) UButton* Slot2Button;
	UPROPERTY(meta=(BindWidget)) UTextBlock* Slot2Name;
	UPROPERTY(meta=(BindWidget)) UTextBlock* Slot2Class;
	
	
};
