#include "UI/Base/RBaseOutGameWidget.h"
#include "Components/Button.h"
#include "Core/RGameInstance.h"
#include "UI/Manager/ROutGameUIManager.h"
#include "Kismet/GameplayStatics.h"

URBaseOutGameWidget::URBaseOutGameWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void URBaseOutGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	//키 입력 받기 위해 포커스 설정
	SetKeyboardFocus();
	
	UE_LOG(LogTemp,Log,TEXT("BaseOutGameWidget is created"));
}

void URBaseOutGameWidget::ShowUI()
{
	//화면에 추가
	AddToViewport();

	//입력 모드를 UI전용으로 변경
	if (APlayerController* PC=GetOwningPlayer())
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor=true;
	}

	SetKeyboardFocus();
	
	UE_LOG(LogTemp,Log,TEXT("%s 표시됨"),*GetClass()->GetName());
}

FReply URBaseOutGameWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	// ESC키 눌럿을떄 검사
	if (InKeyEvent.GetKey()==EKeys::Escape)
	{
		if (CanGoBack())
		{
			// 직접 UI Manager call
			URGameInstance* GI=Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (GI && GI->GetUIManager())
			{
				GI->GetUIManager()->GoBackToPreviousUI();
				UE_LOG(LogTemp,Log,TEXT("뒤로가기 키 입력"));
			}
			return FReply::Handled();
		}
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void URBaseOutGameWidget::HideUI()
{
	//화면에서 제거
	RemoveFromParent();
	UE_LOG(LogTemp, Log, TEXT("%s 숨김"), *GetClass()->GetName());
}

