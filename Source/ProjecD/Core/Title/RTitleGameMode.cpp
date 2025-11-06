// RTitleGameMode.cpp

#include "Core/Title/RTitleGameMode.h"
#include "Core/RGameInstance.h"
#include "UI/Manager/ROutGameUIManager.h"
#include "UI/Base/RBaseOutGameWidget.h"

void ARTitleGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("[RTitleGameMode] 타이틀 맵 시작"));
}

void ARTitleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp,Log,TEXT("[Title] PostLogin 호출 - PlayerController 준비완료"));

	//1. GameInstance 가져오기
	URGameInstance* GameInstance=Cast<URGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		UE_LOG(LogTemp,Error,TEXT("[Title] GameInstance 못찾음!!"));
		return;
	}

	//2. UIManager 가져오기
	UROutGameUIManager* UIManager=GameInstance->GetUIManager();
	if (!UIManager)
	{
		UE_LOG(LogTemp,Error,TEXT("[Title] UIManager 못찾음!!"));
		return;
	}

	//3. UIManager에 PlayerController설정
	if (NewPlayer)
	{
		UIManager->SetPlayerController(NewPlayer);
		UE_LOG(LogTemp,Log,TEXT("[Title] UIManager에 PlayerController 설정!!"));
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("[Title] NewPlayer가 nullptr"));
		return;
	}

	//4. TitleWidgetClass가 설정된건지 획인
	if (!TitleWidgetClass)
	{
		UE_LOG(LogTemp,Error,TEXT("[Title] TitleWidgetClass 설정안됨!!"));
		return;
	}

	//5. Title UI 표시
	UIManager->ShowUI(TitleWidgetClass);
	UE_LOG(LogTemp,Log,TEXT("[Title] 타이틀 UI 표시 완료"))
}
