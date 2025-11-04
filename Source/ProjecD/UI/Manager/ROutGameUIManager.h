#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ROutGameUIManager.generated.h"

/**
 * Outgame UI 생성, 전환, 스택 관리 클래스
 */
UCLASS()
class PROJECD_API UROutGameUIManager : public UObject
{
	GENERATED_BODY()

public:
	UROutGameUIManager(); //초기화

	// PlayerController 설정 및 GameInstance가 Init()에서 호출
	void SetPlayerController(APlayerController* PC);

	// UI 관리 함수 //
	
	// UI위젯 생성 및 화면 표시
	UFUNCTION(BlueprintCallable,Category="UI")
	void ShowUI(TSubclassOf<URBaseOutGameWidget> WidgetClass);

	//현재 활성화된 UI 숨기기
	UFUNCTION(BlueprintCallable,Category="UI")
	void HideCurrentUI();

	//이전 UI로 돌아가기(스택에서 pop)
	UFUNCTION(BlueprintCallable,Category="UI")
	void GoBackToPreviousUI();

	//모든 UI 제거 & 스택 초기화
	UFUNCTION(BlueprintCallable,Category="UI")
	void ClearAllUI();

	//Getter함수

	// 현재 활성화된 UI반환
	UFUNCTION(BlueprintCallable,Category="UI")
	URBaseOutGameWidget* GetCurrentWidget() const {return CurrentWidget;}
	
private:
	//내부 헬퍼함수
	// 위젯을 생성하거나 캐시에서 가져옴
	URBaseOutGameWidget* GetOrCreateWidget(TSubclassOf<URBaseOutGameWidget> WidgetClass);

	//맴버 변수

	// 현재 활성화된 UI위젯
	UPROPERTY()
	URBaseOutGameWidget* CurrentWidget;

	//UI 스택 (뒤로가기 기능관련)
	UPROPERTY()
	TArray<URBaseOutGameWidget*> WidgetStack;

	//생성된 위젯 캐시 (재사용) - Key: 위젯 클래스, Value: 위젯 인스턴스/
	UPROPERTY()
	TMap<TSubclassOf<URBaseOutGameWidget>,URBaseOutGameWidget*> CachedWidgets;

	//PlayerController참조(입력 모드 변경용)
	UPROPERTY()
	APlayerController* PlayerController;

	
};
