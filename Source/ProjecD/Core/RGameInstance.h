#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RGameInstance.generated.h"

class UROutGameUIManager;
class URBaseOutGameWidget;
class URSlotSelectWidget;
class URClassDetailWidget;
class URNameInputWidget;
class UROutGameCharacterDataSubsystem;
class USoundBase;
class UAudioComponent;

//캐릭터 직업 열거형
UENUM(BlueprintType)
enum class ECharacterClassType:uint8
{
	None UMETA(DisplayName="None"),
	Knight UMETA(DisplayName="Knight"),
	Archer UMETA(DisplayName="Archer"),
	Mage UMETA(DisplayName="Mage")
};

//enum class 연산자 
FORCEINLINE bool operator==(ECharacterClassType A, ECharacterClassType B)
{
	return static_cast<uint8>(A) == static_cast<uint8>(B);
}

FORCEINLINE bool operator!=(ECharacterClassType A, ECharacterClassType B)
{
	return !(A==B);
}

//캐릭터 슬롯 데이터 구조체
USTRUCT(BlueprintType)
struct FCharacterSlotData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="Character")
	bool bIsCreated = false;

	UPROPERTY(BlueprintReadWrite, Category="Character")
	ECharacterClassType CharacterClass = ECharacterClassType::None;

	UPROPERTY(BlueprintReadWrite, Category="Character")
	FString CharacterName;

	UPROPERTY(BlueprintReadWrite, Category="Character")
	int32 Level = 1;

	//추후 골드, 경험치 추가
};

//게임 인스턴스
UCLASS()
class PROJECD_API URGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 생성자 및 초기화
	URGameInstance();
	
	virtual void Init() override;
	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;
	
//-------UI 관리----------

	//UI 매니저 접근
	UFUNCTION(BlueprintCallable, Category="UI")
	UROutGameUIManager* GetUIManager() const { return UIManager; }

	//UI 표시 함수
	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowTitleScreen();

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowSlotSelectUI();

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowClassSelectUI();

	UFUNCTION(BlueprintCallable,Category="UI")
	void ShowClassDetailUI();

	UFUNCTION(BlueprintCallable,Category="UI")
	void ShowNameInputUI();

	UFUNCTION(BlueprintCallable,Category="UI")
	void ShowQuitConfirmUI();

//--------음악 관리 --------
//---레벨별 BGM 관리
	//타이틀 레벨용
	void PlayTitleBGM(UWorld* InWorld);
	//로비 로벨용
	void PlayLobbyBGM(UWorld* InWorld);

	//BGM관리
	UFUNCTION(BlueprintCallable,Category="Audio")
	void PlayOutGameBGM();

	UFUNCTION(BlueprintCallable,Category="Audio")
	void StopOutGameBGM();
	

//-------모드------------
	//솔로 파티여부
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mode")
	bool bIsSoloMode = true;

	//현재 파티원 수
	UPROPERTY(BlueprintReadOnly, Category = "Mode")
	int32 PartyCount = 0;

//-------설정------------
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
	
//--------유틸리티 함수--------------
	// 직업 열거형 -> 문자열 변환
	FString GetClassName(ECharacterClassType Class) const;
	
//--------서브 시스템 접근 함수
	UFUNCTION(BlueprintCallable,Category="Character",meta=(WorldContext="WorldContextObject"))
	static UROutGameCharacterDataSubsystem* GetCharacterDataSubsystem(const UObject* WorldContextObject);

//--------- UI 위젯 클래스들----------
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<URBaseOutGameWidget> TitleScreenClass;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<URSlotSelectWidget> SlotSelectWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<URBaseOutGameWidget> ClassSelectWidgetClass;

	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<URClassDetailWidget> ClassDetailWidgetClass;

	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<URBaseOutGameWidget> NameInputWidgetClass;

	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<URBaseOutGameWidget> QuitConfirmWidgetClass;
	
	//음악 에셋
	UPROPERTY(EditDefaultsOnly,Category="Audio")
	USoundBase* OutGameBGM;

	UPROPERTY(EditDefaultsOnly,Category="Audio")
	USoundBase* LobbyBGM;

private:
	void InitializeUIManager();

	UPROPERTY()
	UROutGameUIManager* UIManager;

	UPROPERTY()
	UAudioComponent* BGMComponent;
	
};