#include "Core/RGameInstance.h"
#include "Core/Subsystem/ROutGameCharacterDataSubsystem.h"
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
	InitializeUIManager();
}

void URGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Super::OnWorldChanged(OldWorld, NewWorld);

	// 새로운 월드가 유효한지 검증
	if (NewWorld && NewWorld->IsGameWorld())
	{
		UE_LOG(LogTemp,Warning,TEXT("새로운 월드 진입: %s"),*NewWorld->GetName());
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

void URGameInstance::ShowClassSelectUI()
{
	if (!UIManager)
	{
		UE_LOG(LogTemp,Error,TEXT("UIManager가 Nullptr!!"));
		return;
	}

	if (ClassSelectWidgetClass)
	{
		UIManager->ShowUI(ClassSelectWidgetClass);
		UE_LOG(LogTemp,Log,TEXT("직업 선택 UI 표시!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ClassSelectWidgetClass 설정 안됨!!"));
	}
}

void URGameInstance::ShowNameInputUI()
{
	if (!UIManager)
	{
		UE_LOG(LogTemp,Error,TEXT("UIManager가 Nullptr!!"));
		return;
	}
	if (NameInputWidgetClass)
	{
		UIManager->ShowUI(NameInputWidgetClass);
		UE_LOG(LogTemp,Log,TEXT("닉네임 입력 UI 표시"));
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("NameInputWidgetClass 설정 안됨!!"));
	}
}

void URGameInstance::ShowQuitConfirmUI()
{
	if (!UIManager)
	{
		UE_LOG(LogTemp,Error,TEXT("UIManager가 Nullptr"));
		return;
	}

	if (QuitConfirmWidgetClass)
	{
		UIManager->ShowUI(QuitConfirmWidgetClass);
		UE_LOG(LogTemp,Log,TEXT("Quit 확인 팝업 표시"));
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("QuitConfirmWidgetClass 설정 안됨!!"));
	}
}

FString URGameInstance::GetClassName(ECharacterClassType Class) const
{
	switch (Class)
	{
	case ECharacterClassType::Knight: return TEXT("Knight");
	case ECharacterClassType::Archer: return TEXT("Archer");
	case ECharacterClassType::Mage:   return TEXT("Mage");
	case ECharacterClassType::None:
	default:
		return TEXT("None");
	}
}

UROutGameCharacterDataSubsystem* URGameInstance::GetCharacterDataSubsystem(const UObject* WorldContextObject)
{
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		if (const UGameInstance* GameInstance = World->GetGameInstance())
		{
			return GameInstance->GetSubsystem<UROutGameCharacterDataSubsystem>();
		}
	}
	return nullptr;
}