#include "UI/Popup/RQuitConfirmWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Manager/ROutGameUIManager.h"


void URQuitConfirmWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (YesButton)
	{
		YesButton->OnClicked.Clear();
		YesButton->OnClicked.AddDynamic(this,&URQuitConfirmWidget::OnYesButtonClicked);
		UE_LOG(LogTemp,Log,TEXT("YesButton 바인딩 완!"));
	}
	if (NoButton)
	{
		NoButton->OnClicked.Clear();
		NoButton->OnClicked.AddDynamic(this,&URQuitConfirmWidget::OnNoButtonClicked);
		UE_LOG(LogTemp,Log,TEXT("NoButton 바인딩 완!"));
	}
}

void URQuitConfirmWidget::InitializeUI()
{
	Super::InitializeUI();

	UE_LOG(LogTemp,Log,TEXT("Quit Confirmation UI 초기화"));

	if (TitleText)
	{
		TitleText->SetText(FText::FromString(TEXT("QUIT THE GAME?")));
	}

	if (MessageText)
	{
		MessageText->SetText(FText::FromString(TEXT("Are you sure want to quit this game?")));
	}
}

void URQuitConfirmWidget::CleanupUI()
{
	Super::CleanupUI();

	UE_LOG(LogTemp,Log,TEXT("Quit Confirmation UI 정리"));
}

void URQuitConfirmWidget::ShowUI()
{
	InitializeUI(); // UI 초기화
	AddToViewport(100); // 화면에 ZOrder설정 추가해서 최상단 노출

	// 입력모드를 UI전용으로 변경
	if (APlayerController* PC=GetOwningPlayer())
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor=true;
	}

	SetIsFocusable(true);
	SetKeyboardFocus();

	UE_LOG(LogTemp, Log, TEXT("%s 표시됨 (ZOrder: 100)"), *GetClass()->GetName());
}

void URQuitConfirmWidget::OnYesButtonClicked()
{
	UE_LOG(LogTemp,Warning,TEXT("게임종료 확인"))

	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		GetOwningPlayer(),
		EQuitPreference::Quit,
		false
		);
}

void URQuitConfirmWidget::OnNoButtonClicked()
{
	UE_LOG(LogTemp,Warning,TEXT("게임 종료 취소! UI 닫음!!"));
	
	HideUI();

	if (UROutGameUIManager* UIManager=GetUIManager())
	{
		UIManager->GoBackToPreviousUI();
	}
}
