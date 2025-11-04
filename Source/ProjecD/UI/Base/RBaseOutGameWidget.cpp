#include "UI/Base/RBaseOutGameWidget.h"

URBaseOutGameWidget::URBaseOutGameWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void URBaseOutGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
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
	UE_LOG(LogTemp,Log,TEXT("%s 표시됨"),*GetClass()->GetName());
}

void URBaseOutGameWidget::HideUI()
{
	//화면에서 제거
	RemoveFromParent();
	UE_LOG(LogTemp, Log, TEXT("%s 숨김"), *GetClass()->GetName());
}