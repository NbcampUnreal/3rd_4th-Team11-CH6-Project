// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/RPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"          // GetCurrentLevelName
#include "GameFramework/PlayerController.h"

ARPlayerController::ARPlayerController() 
	: DefaultMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	LobbyHUDClass(nullptr), 
	InGameHUDClass(nullptr), 
	CurrentHUD(nullptr)
{
	const int32 LocalSkillSlots = 3;
	SkillInputActions.SetNumZeroed(LocalSkillSlots);
}

void ARPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if(ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if(DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext,0);
			}
		}
	}
	// HUD 세팅 (맵 이름 보고 어떤 HUD 띄울지 결정)
	SetupHUDForCurrentLevel();
}


/*예시)
* 타이틀 맵 이름:Title_Level → LevelName.Contains("Title") 에 걸림 → HUD 안 만들고 OutGameUIManager가 알아서
* 로비 맵 이름: Lobby_Level → LevelName.Contains("Lobby") 에 걸림 → WBP_LobbyHUD 생성 + UI 모드
* 던전 맵 이름들:Dungeon_01, Dungeon_02, Dungeon_Boss … → Title도 Lobby도 안 들어가니까→ 전부 인게임 HUD(WBP_InGameHUD) 생성
*/ 
void ARPlayerController::SetupHUDForCurrentLevel()
{
	if (!IsLocalController())
	{
		return;
	}

	// 기존 HUD 제거
	if (CurrentHUD)
	{
		CurrentHUD->RemoveFromParent();
		CurrentHUD = nullptr;
	}

	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this, true); // PIE prefix 제거
	UE_LOG(LogTemp, Log, TEXT("[ARPlayerController] 현재 맵: %s"), *LevelName);

	// === 타이틀 맵은 OutGameUIManager가 처리하니까 여기서는 HUD 생성 안 함 ===
	if (LevelName.Contains(TEXT("Title")))
	{
		UE_LOG(LogTemp, Log, TEXT("[ARPlayerController] Title 맵 - InGame HUD 생성 안 함"));
		return;
	}

	// === 로비 맵 ===
	if (LevelName.Contains(TEXT("Lobby")))
	{
		if (LobbyHUDClass)
		{
			CurrentHUD = CreateWidget<UUserWidget>(this, LobbyHUDClass);
			if (CurrentHUD)
			{
				CurrentHUD->AddToViewport();
				EnterUIMode(CurrentHUD);      // 마우스 + UI 모드
				UE_LOG(LogTemp, Log, TEXT("[ARPlayerController] LobbyHUD 생성 완료"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[ARPlayerController] LobbyHUDClass 미설정"));
		}
		return;
	}

	// === 던전/인게임 맵 (예: Dungeon, Stage 이름 포함) ===
	if (InGameHUDClass)
	{
		CurrentHUD = CreateWidget<UUserWidget>(this, InGameHUDClass);
		if (CurrentHUD)
		{
			CurrentHUD->AddToViewport();
			EnterGameControlMode();          // 게임 조작 모드
			UE_LOG(LogTemp, Log, TEXT("[ARPlayerController] InGameHUD 생성 완료"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[ARPlayerController] InGameHUDClass 미설정"));
	}
}

void ARPlayerController::EnterGameControlMode()
{
	if (!IsLocalPlayerController()) return;

	bShowMouseCursor = false;

	FInputModeGameOnly Mode;
	SetInputMode(Mode);

	UE_LOG(LogTemp,Warning,TEXT("[ARPlayerController] 게임만 커서 OFF"));
}

void ARPlayerController::EnterUIMode(UUserWidget* FocusWidget)
{
	if (!IsLocalController()) return;

	bShowMouseCursor = true;

	FInputModeGameAndUI Mode;
	if (FocusWidget)
	{
		Mode.SetWidgetToFocus(FocusWidget->TakeWidget());
	}
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(Mode);

	UE_LOG(LogTemp, Log, TEXT("[ARPlayerController] EnterUIMode: GameAndUI + Cursor ON"));
}