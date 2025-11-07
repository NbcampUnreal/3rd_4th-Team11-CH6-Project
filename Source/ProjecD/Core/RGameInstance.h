#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RGameInstance.generated.h"

class UROutGameUIManager;
class URBaseOutGameWidget;
class URSlotSelectWidget;

//캐릭터 직업 열거형- SKill에서 Class를 써서 뒤에 구분되게 ClassType으로 씀
UENUM(BlueprintType)
enum class ECharacterClassType:uint8
{
	None UMETA(DisplayName="None"),
	Knight UMETA(DisplayName="Knight"),
	Archer UMETA(DisplayName="Archer"),
	Mage UMETA(DisplayName="Mage")
};

//enum class 연산자 만들기
FORCEINLINE bool operator==(ECharacterClassType A,ECharacterClassType B)
{
	return static_cast<uint8>(A) == static_cast<uint8>(B);
}

FORCEINLINE bool operator!=(ECharacterClassType A,ECharacterClassType B)
{
	return !(A==B);
}

//캐릭터 슬롯 데이터 구조체
USTRUCT(BlueprintType)
struct FCharacterSlotData
{
	GENERATED_BODY()

	//캐릭터 생성여부
	UPROPERTY(BlueprintReadWrite,Category="Character")
	bool bIsCreated=false;

	//캐릭터 직업
	UPROPERTY(BlueprintReadWrite,Category="Character")
	ECharacterClassType CharacterClass=ECharacterClassType::None;

	//캐릭터 이름
	UPROPERTY(BlueprintReadWrite,Category="Character")
	FString CharacterName;

	//캐릭터 레벨
	UPROPERTY(BlueprintReadWrite,Category="Character")
	int32 Level=1;

	//추후에 골드나 경험치 부분 추가
};

//게임 인스턴스
UCLASS()
class PROJECD_API URGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	URGameInstance();
	virtual void Init() override;
	virtual void OnWorldChanged(UWorld* OldWorld,UWorld* NewWorld) override; // 멀티플레이어 설정

	// UI팀 관련 설정 
	UFUNCTION(BlueprintCallable,Category="UI")
	UROutGameUIManager* GetUIManager() const { return UIManager; }

	UFUNCTION(BlueprintCallable,Category="UI")
	void ShowTitleScreen(); // 타이틀 화면 표시

	UFUNCTION(BlueprintCallable,Category="UI")
	void ShowSlotSelectUI();

//---------모드------------
	//솔로 파티여부
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mode")
	bool bIsSoloMode = true;

	//현재 파티원 수
	UPROPERTY(BlueprintReadOnly, Category = "Mode")
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

	// 캐릭터 슬롯 시스템 - 직업별 1개 제한!)
	UPROPERTY(BlueprintReadWrite,Category="Character")
	TArray<FCharacterSlotData> CharacterSlots; //캐릭터 슬롯 데이터 (나이트,아처,메이지 총 3개)
	
	UPROPERTY(BlueprintReadWrite,Category="Character")
	int32 SelectedCharacterIndex=-1; //현재 선택딘 캐릭터 인덱스 (-1인 경우 선택 X)

	// 직업 제한 함수
	UFUNCTION(BlueprintCallable,Category="Character")
	bool HasCharacterOfClass(ECharacterClassType Class) const;

	UFUNCTION(BlueprintCallable,Category="Character")
	int32 FindAvailableSlotForClass(ECharacterClassType Class) const;

	// 캐릭터 관리 //
	
	// 캐릭터 닉네임 유효성 검사
	UFUNCTION(BlueprintCallable,Category="Character")
	bool IsValidCharacterName(const FString& Name,FString& OutErrorMessage) const; 
	
	UFUNCTION(BlueprintCallable,Category="Character")
	bool CreateCharacter(int32 TargetSlotIndex,ECharacterClassType Class,const FString& Name);

	//캐릭터 선택, return은 성공 여부
	UFUNCTION(BlueprintCallable,Category="Character")
	bool SelectCharacter(int32 SlotIndex);

	//생성된 캐릭터가 하나라도 있는지 체크, return은 캐릭터 존재여부
	UFUNCTION(BlueprintCallable,Category="Character")
	bool HasAnyCharacter() const;

	//특정 슬롯이 비어있는지 체크, return은 빈 슬롯 여부
	UFUNCTION(BlueprintCallable,Category="Character")
	bool IsSlotEmpty(int32 SlotIndex) const;

	//특정 슬롯의 캐릭터 데이터를 가져오기, return은 캐릭터 데이터
	UFUNCTION(BlueprintCallable,Category="Character")
	FCharacterSlotData GetCharacterData(int32 SlotIndex) const;

	//현재 선택된 캐릭터 데이터 가져오기, return은 선택된 캐릭터 데이터
	UFUNCTION(BlueprintCallable,Category="Character")
	FCharacterSlotData GetSelectedCharacterData() const;

	//캐릭터 삭제, return은 성공 여부
	UFUNCTION(BlueprintCallable,Category="Character")
	bool DeleteCharacter(int32 SlotIndex);

	//직업열거형->문자열 변환 함수
	FString GetClassName(ECharacterClassType Class) const;

	//UI 위젯 클래스들
	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<URBaseOutGameWidget> TitleScreenClass;

	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<class URSlotSelectWidget> SlotSelectWidgetClass;

private:
	//UI팀 관련 설정 //
	void InitializeUIManager(); // UI 초기화
	void InitializeCharacterSlots(); // 슬롯 초기화
	
	//닉네임 규칙
	static constexpr int32 MIN_NAME_LENGTH = 2; // 최소 길이
	static constexpr int32 MAX_NAME_LENGTH = 10; // 최대 길이

	UPROPERTY()
	UROutGameUIManager* UIManager;
	
};
