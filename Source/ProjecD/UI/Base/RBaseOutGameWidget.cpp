#include "UI/Base/RBaseOutGameWidget.h"
#include "Components/Button.h"
#include "Core/RGameInstance.h"
#include "UI/Manager/ROutGameUIManager.h"
#include "Kismet/GameplayStatics.h"

URBaseOutGameWidget::URBaseOutGameWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SetIsFocusable(true);
}

bool URBaseOutGameWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	
	SetIsFocusable(true);
	return true;
}

void URBaseOutGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	//키 입력 받기 위해 포커스 설정
	SetIsFocusable(true);
	SetKeyboardFocus();

	UE_LOG(LogTemp, Log, TEXT("BaseOutGameWidget created, IsFocusable: %s"), 
		IsFocusable() ? TEXT("True") : TEXT("False"));
	
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

	SetIsFocusable(true);
	SetKeyboardFocus();
	
	UE_LOG(LogTemp,Log,TEXT("%s 표시됨"),*GetClass()->GetName());
}

FReply URBaseOutGameWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("키 입력 감지: %s"), *InKeyEvent.GetKey().ToString());
	//  백스페이스 키를 눌럿을떄
	if (InKeyEvent.GetKey()==EKeys::BackSpace)
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

