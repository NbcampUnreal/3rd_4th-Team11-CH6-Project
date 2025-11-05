#include "UI/Title/RTitleScreenWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Core/RGameInstance.h"
#include "UI/Manager/ROutGameUIManager.h"


void URTitleScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp,Warning,TEXT("타이틀 화면 생성됨!!"));
	
	if (PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this,&URTitleScreenWidget::OnPlayClicked);
		UE_LOG(LogTemp,Log,TEXT("Play버튼 바인딩 완료!!"));
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("Play 버튼이 nullptr!!"));
	}

	if (OptionsButton)
	{
		OptionsButton->OnClicked.AddDynamic(this,&URTitleScreenWidget::OnOptionsClicked);
		UE_LOG(LogTemp,Log,TEXT("Options버튼 바인딩 완료!!"));
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("Options 버튼이 nullptr!!"));
	}

	if (CreditsButton)
	{
		CreditsButton->OnClicked.AddDynamic(this,&URTitleScreenWidget::OnCreditsClicked);
		UE_LOG(LogTemp,Log,TEXT("Credits버튼 바인딩 완료!!"));
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("Credits 버튼이 nullptr"));
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this,&URTitleScreenWidget::OnQuitClicked);
		UE_LOG(LogTemp,Log,TEXT("Quit버튼 바인딩 완료"));
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("Quit버튼이 nullptr!"));
	}
}

void URTitleScreenWidget::OnPlayClicked()
{
	UE_LOG(LogTemp,Warning,TEXT("Play버튼 클릭됨!!"));

	//세션 브라우저 화면으로 전환 -나중에 구현
	// UE_UE_LOG(LogTemp, Warning, TEXT("[RTitleScreenWidget] 세션 브라우저로 이동"));
	
}

void URTitleScreenWidget::OnOptionsClicked()
{
	UE_LOG(LogTemp,Warning,TEXT("Options 버튼 클릭됨!!"));
	
}

void URTitleScreenWidget::OnCreditsClicked()
{
	UE_LOG(LogTemp,Warning,TEXT("Credits 버튼 클릭됨!!"));
}

void URTitleScreenWidget::OnQuitClicked()
{
	UE_LOG(LogTemp,Warning,TEXT("Quit 버튼 클릭됨!!"));

	// 게임 종료
	if (APlayerController* PC=GetOwningPlayer())
	{
		UKismetSystemLibrary::QuitGame(
			GetWorld(),
			PC,
			EQuitPreference::Quit,
			false
			);

		UE_LOG(LogTemp,Warning,TEXT("[Title] 게임 종료 요청 완료!!"));
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("[Title] PlayerController를 찾을수 없음!!"));
	}
}
