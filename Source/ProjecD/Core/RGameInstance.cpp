#include "Core/RGameInstance.h"
#include "UI/Manager/ROutGameUIManager.h"
#include "UI/Base/RBaseOutGameWidget.h"
#include "UI/Slot/RSlotSelectWidget.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

URGameInstance::URGameInstance()
{
	//나중에 기본적인 것추가
}

void URGameInstance::Init()
{
	Super::Init();
	UIManager=NewObject<UROutGameUIManager>(this);
	InitializeCharacterSlots();
}

void URGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Super::OnWorldChanged(OldWorld, NewWorld);

	// 새로운 월드가 유효한지 검증
	if (NewWorld && NewWorld->IsGameWorld())
	{
		UE_LOG(LogTemp,Warning,TEXT("새로운 월드 진입: %s"),*NewWorld->GetName());
		InitializeUIManager();
	}
}

void URGameInstance::InitializeUIManager()
{
	if (!UIManager)
	{
		UE_LOG(LogTemp,Error,TEXT("UIManager is Nullptr!!"));
		return;
	}

	if (UWorld* World=GetWorld())
	{
		if (APlayerController* PC=UGameplayStatics::GetPlayerController(World,0))
		{
			UIManager->SetPlayerController(PC);
			UE_LOG(LogTemp,Warning,TEXT("UIManager에 PlayerController 설정 완료!!"));
		}
		else
		{
			UE_LOG(LogTemp,Error,TEXT("PlayerController 찾을수 없음!!"));
		}
	}
}

void URGameInstance::InitializeCharacterSlots()
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

void URGameInstance::ShowTitleScreen()
{
	if (!UIManager)
	{
		UE_LOG(LogTemp,Error,TEXT("UIManager 가 Nullptr!!"));
		return;
	}
	if (!TitleScreenClass)
	{
		UE_LOG(LogTemp,Error,TEXT("TitleScreenClass가 설정 안됨!!"));
		return;
	}

	UIManager->ShowUI(TitleScreenClass);
}

void URGameInstance::ShowSlotSelectUI()
{
	if (!UIManager)
	{
		UE_LOG(LogTemp,Error,TEXT("UIManager가 Nullptr!!"));
		return;
	}

	if (SlotSelectWidgetClass)
	{
		UIManager->ShowUI(SlotSelectWidgetClass);
		UE_LOG(LogTemp,Log,TEXT("슬롯 선택 UI 표시"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SlotSelectWidgetClass 설정 안됨!!"));
	}
}

bool URGameInstance::IsValidCharacterName(const FString& Name, FString& OutErrorMessage) const
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
		
		//한글 범위 체크
		bool bIsKorean=(Char >= 0xAC00 && Char <= 0xD7A3);
		
		//영문 대소문자 체크
		bool bIsEnglish=(Char>='A' && Char <='Z') || (Char>= 'a' && Char <='z');

		//숫자 체크
		bool bIsNumber=(Char>='0' && Char <='9');

		//허용되지 않는 문자 발견
		if (!bIsKorean && !bIsEnglish && !bIsNumber)
		{
			OutErrorMessage=TEXT("닉네임은 한글,영문,숫자만 사용할 수 있습니다.");
			UE_LOG(LogTemp,Warning,TEXT("유효하지 않은 문자 발견: '%c' (Code:%d)"),Char,(int32)Char);
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
		if (Slot.bIsCreated && Slot.CharacterName.Equals(TrimmedName))
		{
			OutErrorMessage=TEXT("이미 사용 중인 닉네입니다.");
			return false;
		}
	}

	//모든 검사 통과시
	OutErrorMessage=TEXT("");
	return true;
}

bool URGameInstance::CreateCharacter(int32 SlotIndex, ECharacterClassType Class, const FString& Name)
{
	//유효성 검사 1: 슬롯 인덱스
	if (SlotIndex<0 || SlotIndex >= CharacterSlots.Num())
	{
		UE_LOG(LogTemp,Error,TEXT("유효하지않은 슬롯 인덱스: %d"),SlotIndex);
		return false;
	}
	
	//유효성 검사2: 이미 생성된 캐릭터
	if (CharacterSlots[SlotIndex].bIsCreated)
	{
		UE_LOG(LogTemp,Warning,TEXT("슬롯 %d 이미 캐릭터가 있습니다: %s"),SlotIndex,*CharacterSlots[SlotIndex].CharacterName);
		return false;
	}

	//유효성 검사3: 직업
	if (Class==ECharacterClassType::None)
	{
		UE_LOG(LogTemp,Error,TEXT(" 유요하지 않은 직업군입니다!"));
		return false;
	}

	//유효성 검사4: 닉네임 규칙
	FString ErrorMessage;
	if (!IsValidCharacterName(Name,ErrorMessage))
	{
		UE_LOG(LogTemp,Error,TEXT("유요하지않은 캐릭터 이름: %s"),*ErrorMessage);
		return false;
	}

	FString TrimmedName=Name.TrimStartAndEnd(); //이름 공백 제거

	//캐릭터 생성
	CharacterSlots[SlotIndex].bIsCreated=true; //생성됨 표시
	CharacterSlots[SlotIndex].CharacterClass=Class; // 직업 저장
	CharacterSlots[SlotIndex].CharacterName=TrimmedName; // 이름 저장
	CharacterSlots[SlotIndex].Level=1; // 레벨 1로 시작

	//직업이름을 문자열로 전환
	FString ClassName;
	switch (Class)
	{
	case ECharacterClassType::Knight: ClassName=TEXT("Knight"); break;
	case ECharacterClassType::Archer: ClassName=TEXT("Archer"); break;
	case ECharacterClassType::Mage:   ClassName=TEXT("Mage"); break;
	default:                      ClassName=TEXT("None"); break;
	}

	UE_LOG(LogTemp,Log,TEXT("캐릭터 생성됨! Slot: %d, Class: %s. Name: '%s'"),SlotIndex,*ClassName,*TrimmedName);
	return true;
}

bool URGameInstance::SelectCharacter(int32 SlotIndex)
{
	//유효성 검사1: 슬롯인덱스
	if (SlotIndex <0 || SlotIndex >=CharacterSlots.Num())
	{
		UE_LOG(LogTemp,Error,TEXT("유요하지 않은 슬롯 인덱스: %d"),SlotIndex);
		return false;
	}

	//유효성 검사2: 빈 슬롯
	if (!CharacterSlots[SlotIndex].bIsCreated)
	{
		UE_LOG(LogTemp,Error,TEXT("슬롯 %d가 비었습니다"),SlotIndex);
		return false;
	}

	//캐릭터 선택
	SelectedCharacterIndex=SlotIndex;

	UE_LOG(LogTemp,Log,TEXT("캐릭터 선택됨! Slot: %d, Name:'%s'"),SlotIndex,*CharacterSlots[SlotIndex].CharacterName);
	return true;
}

bool URGameInstance::HasAnyCharacter() const
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

bool URGameInstance::IsSlotEmpty(int32 SlotIndex) const
{
	if (SlotIndex<0 || SlotIndex>=CharacterSlots.Num())
	{
		return true;
	}
	return !CharacterSlots[SlotIndex].bIsCreated;
}

FCharacterSlotData URGameInstance::GetCharacterData(int32 SlotIndex) const
{
	if (SlotIndex<0 || SlotIndex >= CharacterSlots.Num())
	{
		UE_LOG(LogTemp,Error,TEXT("유요하지 않은 슬롯 인덱스: %d"),SlotIndex);
		return FCharacterSlotData();
	}
	return CharacterSlots[SlotIndex];
}

FCharacterSlotData URGameInstance::GetSelectedCharacterData() const
{
	if (SelectedCharacterIndex<0 || SelectedCharacterIndex>=CharacterSlots.Num())
	{
		UE_LOG(LogTemp,Warning,TEXT("캐릭터 선택 안됨!!"));
		return FCharacterSlotData();
	}
	return CharacterSlots[SelectedCharacterIndex];
}

bool URGameInstance::DeleteCharacter(int32 SlotIndex)
{
	//유효성 검사1: 슬롯 인덱스
	if (SlotIndex <0 || SlotIndex >= CharacterSlots.Num())
	{
		UE_LOG(LogTemp,Error,TEXT("유효하지 않은 슬롯 인덱스: %d"),SlotIndex);
		return false;
	}

	//유효성 검사2: 빈 슬롯
	if (!CharacterSlots[SlotIndex].bIsCreated)
	{
		UE_LOG(LogTemp,Warning,TEXT("슬롯 %d 가 이미 비어있습니다!"),SlotIndex);
		return false;
	}

	//캐릭터 삭제
	FString DeletedName=CharacterSlots[SlotIndex].CharacterName;
	CharacterSlots[SlotIndex].bIsCreated=false;
	CharacterSlots[SlotIndex].CharacterClass=ECharacterClassType::None;
	CharacterSlots[SlotIndex].CharacterName=TEXT("");
	CharacterSlots[SlotIndex].Level=1;

	//선택된 캐릭터 였다면 선택 헤제
	if (SelectedCharacterIndex==SlotIndex)
	{
		SelectedCharacterIndex=-1;
	}

	UE_LOG(LogTemp,Log,TEXT("캐릭터 삭제됨!! Slot: %d, Name:'%s'"),SlotIndex,*DeletedName);
	return true;
}
