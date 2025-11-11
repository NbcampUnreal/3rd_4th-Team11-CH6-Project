#include "Core/Subsystem/ROutGameCharacterDataSubsystem.h"
#include "Core/RGameInstance.h"
#include "Misc/Char.h"


void UROutGameCharacterDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeCharacterSlots();

	UE_LOG(LogTemp, Log, TEXT("ROutGameCharacterDataSubsystem 초기화 및 슬롯 설정 완료."));
}

void UROutGameCharacterDataSubsystem::Deinitialize()
{
	Super::Deinitialize();
	UE_LOG(LogTemp, Log, TEXT("ROutGameCharacterDataSubsystem 종료."));
}

void UROutGameCharacterDataSubsystem::InitializeCharacterSlots()
{
	// 캐릭터 슬롯 3개 초기화 (Knight,Archer,Mage)
	CharacterSlots.SetNum(3);
	for (int32 i=0; i<3; i++)
	{
		CharacterSlots[i].bIsCreated=false; 
		CharacterSlots[i].CharacterClass=ECharacterClassType::None; 
		CharacterSlots[i].CharacterName=TEXT(""); 
		CharacterSlots[i].Level=1; 
	}
	SelectedCharacterIndex=-1; 
	UE_LOG(LogTemp,Warning,TEXT("캐릭터 슬롯 초기화: %d slots"),CharacterSlots.Num()); 
}

bool UROutGameCharacterDataSubsystem::HasAnyCharacter() const
{
	for (const FCharacterSlotData& Slot: CharacterSlots)
	{
		if (Slot.bIsCreated)
		{
			return true;
		}
	}
	return false;
}

bool UROutGameCharacterDataSubsystem::HasCharacterOfClass(ECharacterClassType Class) const
{
	for (const FCharacterSlotData& Slot: CharacterSlots)
	{
		if (Slot.bIsCreated && Slot.CharacterClass==Class)
		{
			return true;
		}
	}
	return false;
}

bool UROutGameCharacterDataSubsystem::IsSlotEmpty(int32 SlotIndex) const
{
	if (SlotIndex<0 || SlotIndex>=CharacterSlots.Num())
	{
		return true;
	}
	return !CharacterSlots[SlotIndex].bIsCreated;
}

int32 UROutGameCharacterDataSubsystem::FindAvailableSlotForClass(ECharacterClassType Class) const
{
	if (HasCharacterOfClass(Class))
	{
		return -1; // 이미 해당직업이 있으면 -1 반환
	}

	//빈 슬롯 찾기
	for (int32 i = 0; i < CharacterSlots.Num(); i++)
	{
		if (IsSlotEmpty(i))
		{
			return i;
		}
	}
	return -1;
}

FCharacterSlotData UROutGameCharacterDataSubsystem::GetCharacterData(int32 SlotIndex) const
{
	if (SlotIndex<0 || SlotIndex >= CharacterSlots.Num())
	{
		UE_LOG(LogTemp,Error,TEXT("유요하지 않은 슬롯 인덱스: %d"),SlotIndex); 
		return FCharacterSlotData();
	}
	return CharacterSlots[SlotIndex];
}

FCharacterSlotData UROutGameCharacterDataSubsystem::GetSelectedCharacterData() const
{
	if (SelectedCharacterIndex<0 || SelectedCharacterIndex>=CharacterSlots.Num())
	{
		UE_LOG(LogTemp,Warning,TEXT("캐릭터 선택 안됨!!")); 
		return FCharacterSlotData();
	}
	return CharacterSlots[SelectedCharacterIndex];
}

bool UROutGameCharacterDataSubsystem::IsValidCharacterName(const FString& Name, FString& OutErrorMessage) const
{
	FString TrimmedName=Name.TrimStartAndEnd(); 

	//1. 빈 문자열 검사
	if (TrimmedName.IsEmpty())
	{
		OutErrorMessage=TEXT("닉네임을 입력해주세요.");
		return false;
	}

	//2. 최소 길이 검사
	if (TrimmedName.Len()<MIN_NAME_LENGTH)
	{
		OutErrorMessage=FString::Printf(TEXT("닉네임은 최소 %d자 이상이어야 합니다."),MIN_NAME_LENGTH); 
		return false;
	}

	//3. 최대 길이 검사
	if (TrimmedName.Len()>MAX_NAME_LENGTH)
	{
		OutErrorMessage=FString::Printf(TEXT("닉네임은 최대 %d자까지 가능합니다."),MAX_NAME_LENGTH); 
		return false;
	}

	//4. 허용 문자 검사 (한글 ,영문 ,숫자만)
	for (int32 i=0; i<TrimmedName.Len(); i++)
	{
		TCHAR Char=TrimmedName[i];
		
		bool bIsKorean=(Char >= 0xAC00 && Char <= 0xD7A3); //한글 범위 체크
		bool bIsEnglish=(Char>='A' && Char <='Z') || (Char>= 'a' && Char <='z'); //영문 대소문자 체크
		bool bIsNumber=(Char>='0' && Char <='9'); //숫자 체크

		//허용되지 않는 문자 발견
		if (!bIsKorean && !bIsEnglish && !bIsNumber)
		{
			OutErrorMessage=TEXT("닉네임은 한글,영문,숫자만 사용할 수 있습니다."); 
			return false;
		}
	}

	//5. 공백 검사
	if (TrimmedName.Contains(TEXT(" ")))
	{
		OutErrorMessage=TEXT("닉네임에 공백을 사용할 수 없습니다.");
		return false;
	}

	//6. 중복검사(같은 닉네임이 이미 있는지)
	for (const FCharacterSlotData& Slot: CharacterSlots)
	{
		//bIsCreated가 true이고, 이름이 비어 있지 않는 경우에만 검사
		if (Slot.bIsCreated && !Slot.CharacterName.IsEmpty() && Slot.CharacterName.Equals(TrimmedName))
		{
			OutErrorMessage=TEXT("이미 사용중인 닉네임입니다.");
			UE_LOG(LogTemp,Warning,TEXT("중복 닉네임 발견: %s"),*Slot.CharacterName);
			return false;
		}

	}

	//모든 검사 통과시
	OutErrorMessage=TEXT("");
	return true;
}

bool UROutGameCharacterDataSubsystem::CreateCharacter(int32 SlotIndex, ECharacterClassType Class, const FString& Name)
{
	//유효성 검사 1: 슬롯 인덱스
	if (SlotIndex<0 || SlotIndex >= CharacterSlots.Num())
	{
		return false;
	}
	
	//유효성 검사2: 이미 생성된 캐릭터
	if (CharacterSlots[SlotIndex].bIsCreated)
	{
		return false;
	}

	//유효성 검사3: 직업
	if (Class==ECharacterClassType::None)
	{
		return false;
	}

	//유효성 검사4: 닉네임 규칙
	FString ErrorMessage;
	if (!IsValidCharacterName(Name,ErrorMessage))
	{
		return false;
	}

	//캐릭터 생성
	FString TrimmedName=Name.TrimStartAndEnd(); //이름 공백 제거
	CharacterSlots[SlotIndex].bIsCreated=true; //생성됨 표시
	CharacterSlots[SlotIndex].CharacterClass=Class; // 직업 저장
	CharacterSlots[SlotIndex].CharacterName=TrimmedName; // 이름 저장
	CharacterSlots[SlotIndex].Level=1; // 레벨 1로 시작
	
	return true;
}

bool UROutGameCharacterDataSubsystem::SelectCharacter(int32 SlotIndex)
{
	//유효성 검사1: 슬롯인덱스
	if (SlotIndex <0 || SlotIndex >=CharacterSlots.Num())
	{
		return false;
	}

	//유효성 검사2: 빈 슬롯
	if (!CharacterSlots[SlotIndex].bIsCreated)
	{
		return false;
	}

	//캐릭터 선택
	SelectedCharacterIndex=SlotIndex;
	
	return true;
}

bool UROutGameCharacterDataSubsystem::DeleteCharacter(int32 SlotIndex)
{
	//유효성 검사1: 슬롯 인덱스
	if (SlotIndex <0 || SlotIndex >= CharacterSlots.Num())
	{
		return false;
	}

	//유효성 검사2: 빈 슬롯
	if (!CharacterSlots[SlotIndex].bIsCreated)
	{
		return false;
	}

	//캐릭터 삭제
	CharacterSlots[SlotIndex].bIsCreated=false; 
	CharacterSlots[SlotIndex].CharacterClass=ECharacterClassType::None; 
	CharacterSlots[SlotIndex].CharacterName=TEXT(""); 
	CharacterSlots[SlotIndex].Level=1; 

	//선택된 캐릭터 였다면 선택 해제
	if (SelectedCharacterIndex==SlotIndex)
	{
		SelectedCharacterIndex=-1;
	}
	
	return true;
}