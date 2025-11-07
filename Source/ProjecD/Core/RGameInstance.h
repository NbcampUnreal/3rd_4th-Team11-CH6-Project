#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RGameInstance.generated.h"

class UROutGameUIManager;
class URBaseOutGameWidget;
class URSlotSelectWidget;

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

	//추후 골드, 경험치 추가 ,브랜치갱신을 위한 추가
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
	
///-------UI 관리----------
	UFUNCTION(BlueprintCallable, Category="UI")
	UROutGameUIManager* GetUIManager() const { return UIManager; }

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowTitleScreen();

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowSlotSelectUI();

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowClassSelectUI();

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

//------캐릭터 UI 관련 설정 (슬롯,닉네임등)--------
	
	// 임시 선택 직업 (UI 흐름용)
	UPROPERTY()
	ECharacterClassType TempSelectedClass = ECharacterClassType::None;

	// 캐릭터 슬롯 배열 (Knight, Archer, Mage)
	UPROPERTY(BlueprintReadWrite, Category="Character")
	TArray<FCharacterSlotData> CharacterSlots;

	// 현재 선택된 캐릭터 인덱스 (-1 = 미선택)
	UPROPERTY(BlueprintReadWrite, Category="Character")
	int32 SelectedCharacterIndex = -1;
	
//----------캐릭터 데이터 조회-------------------
	
	// 캐릭터 존재 여부 확인
	UFUNCTION(BlueprintCallable, Category="Character")
	bool HasAnyCharacter() const;

	UFUNCTION(BlueprintCallable, Category="Character")
	bool HasCharacterOfClass(ECharacterClassType Class) const;

	UFUNCTION(BlueprintCallable, Category="Character")
	bool IsSlotEmpty(int32 SlotIndex) const;

	// 슬롯 찾기
	UFUNCTION(BlueprintCallable, Category="Character")
	int32 FindAvailableSlotForClass(ECharacterClassType Class) const;

	// 캐릭터 데이터 가져오기
	UFUNCTION(BlueprintCallable, Category="Character")
	FCharacterSlotData GetCharacterData(int32 SlotIndex) const;

	UFUNCTION(BlueprintCallable, Category="Character")
	FCharacterSlotData GetSelectedCharacterData() const;
	
//----------캐릭터 데이터 조작-------
	
	// 닉네임 유효성 검사
	UFUNCTION(BlueprintCallable, Category="Character")
	bool IsValidCharacterName(const FString& Name, FString& OutErrorMessage) const;

	// 캐릭터 생성
	UFUNCTION(BlueprintCallable, Category="Character")
	bool CreateCharacter(int32 SlotIndex, ECharacterClassType Class, const FString& Name);

	// 캐릭터 선택
	UFUNCTION(BlueprintCallable, Category="Character")
	bool SelectCharacter(int32 SlotIndex);

	// 캐릭터 삭제
	UFUNCTION(BlueprintCallable, Category="Character")
	bool DeleteCharacter(int32 SlotIndex);

//--------유틸리티 함수--------------
	// 직업 열거형 -> 문자열 변환
	FString GetClassName(ECharacterClassType Class) const;
	
//--------- UI 위젯 클래스들----------
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<URBaseOutGameWidget> TitleScreenClass;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<URSlotSelectWidget> SlotSelectWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<URBaseOutGameWidget> ClassSelectWidgetClass;

private:
	void InitializeUIManager();
	void InitializeCharacterSlots();
	
	// 닉네임 규칙
	static constexpr int32 MIN_NAME_LENGTH = 2;
	static constexpr int32 MAX_NAME_LENGTH = 10;

	UPROPERTY()
	UROutGameUIManager* UIManager;
};