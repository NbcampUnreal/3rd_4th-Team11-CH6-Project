#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/RGameInstance.h"
#include "ROutGameCharacterDataSubsystem.generated.h"

/**
 * GameInstance에서의 캐릭터 슬롯 데이터 및 로직을 따로 전담해서 관리함!!
 */
UCLASS()
class PROJECD_API UROutGameCharacterDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// UGameInstanceSubsystem 오버라이드: 생성자 대신 초기화에 사용합니다.
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// UGameInstanceSubsystem 오버라이드: 종료시 호출
	virtual void Deinitialize() override;

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


private:
	//초기화 로직
	void InitializeCharacterSlots();
	
	// 닉네임 규칙
	static constexpr int32 MIN_NAME_LENGTH = 2;
	static constexpr int32 MAX_NAME_LENGTH = 10;

	
};
