#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RGameInstance.generated.h"

class UROutGameUIManager;
class URBaseOutGameWidget;

UCLASS()
class PROJECD_API URGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	URGameInstance();

	virtual void Init() override;

	// 멀티플레이어 설정
	virtual void OnWorldChanged(UWorld* OldWorld,UWorld* NewWorld) override;

	// UI팀  관련 설정
	UFUNCTION(BlueprintCallable,Category="UI")
	UROutGameUIManager* GetUIManager() const { return UIManager; }

	UFUNCTION(BlueprintCallable,Category="UI")
	void ShowTitleScreen(); // 타이틀 화면 표시 함수

//---------모드------------
	//솔로 파티여부
	UPROPERTY(Blueprintreadonly, EditAnywhere, Category = "Mode")
	bool bIsSoloMode = true;

	//현재 파티원 수
	UPROPERTY(Blueprintreadonly, Category = "Mode")
	int32 PartyCount = 0;

//---------설정------------
	//솔로 모드 설정 
	void SetSoloMode(bool bSolo) { bIsSoloMode = bSolo; }

	//현제 솔로 여부 확인
	bool IsSoloMode() const { return bIsSoloMode; }

	//파티 인원 수 갱신 (자동 솔로 모드 전환)
	void SetPartyCount(int32 NewCount)
	{
		PartyCount = NewCount;
		bIsSoloMode = (NewCount <= 1); //인원 1 이하 = 솔로
	}
private:
	//UI팀 관련 설정//
	void InitializeUIManager(); // UIManager 초기화 함수
	
	UPROPERTY()
	UROutGameUIManager* UIManager;

	UPROPERTY(EditDefaultsOnly,Category="UI") // TitleScreen Widget 클래스 참조
	TSubclassOf<URBaseOutGameWidget> TitleScreenClass;
	
};
