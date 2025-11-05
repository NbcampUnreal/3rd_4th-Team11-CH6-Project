#include "Core/RGameInstance.h"
#include "UI/Manager/ROutGameUIManager.h"
#include "UI/Base/RBaseOutGameWidget.h"
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

	if (UIManager)
	{
		UE_LOG(LogTemp,Warning,TEXT("UIManager is created!"));
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("UIManager is not created!"));
	}
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
