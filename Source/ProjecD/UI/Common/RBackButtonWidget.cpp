#include "UI/Common/RBackButtonWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Core/RGameInstance.h"
#include "UI/Manager/ROutGameUIManager.h"


void URBackButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackButtonMain)
	{
		BackButtonMain->OnClicked.Clear();
		BackButtonMain->OnClicked.AddDynamic(this,&URBackButtonWidget::OnBackButtonClicked);
		UE_LOG(LogTemp,Log,TEXT("공통 뒤로가기 버튼 바인딩 완료!"));
	}
}

void URBackButtonWidget::OnBackButtonClicked()
{
	URGameInstance* GI=Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI && GI->GetUIManager())
	{
		GI->GetUIManager()->GoBackToPreviousUI();
		UE_LOG(LogTemp,Log,TEXT("뒤로가기 (버튼 클릭!)"));
	}
}
