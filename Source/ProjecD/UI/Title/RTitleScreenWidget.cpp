#include "UI/Title/RTitleScreenWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Core/RGameInstance.h"
#include "Core/Subsystem/ROutGameCharacterDataSubsystem.h"
#include "UI/Manager/ROutGameUIManager.h"


void URTitleScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UE_LOG(LogTemp,Warning,TEXT("타이틀 화면 생성됨, 포커스 활성화됨!!"));
	
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

	//1. 게임인스턴스 가져오기
	URGameInstance* GI=Cast<URGameInstance>(GetGameInstance());
	if (!GI)
	{
		UE_LOG(LogTemp,Error,TEXT("GameInstance를 찾을수 없음!!"));
		return;
	}

	//2. 캐릭터 데이터 서브시스템 가져오기
	UROutGameCharacterDataSubsystem* Subsystem=URGameInstance::GetCharacterDataSubsystem(this);
	if (!Subsystem)
	{
		UE_LOG(LogTemp,Error,TEXT("CharacterDataSubsystem 접근 실패!"));
		return;
	}

	//3.캐릭터 존재 여부 확인
	if (Subsystem->HasAnyCharacter())
	{
		//캐릭터 있을시, 로비로 바로 이동(미구현)
		UE_LOG(LogTemp,Warning,TEXT("캐릭터가 존재! 인게임 로비로 이동!!"));

		//GI->ShowLobbyUI();
	}
	else
	{
		//캐릭터가 없으면, 슬롯 선택 UI로 이동
		UE_LOG(LogTemp,Warning,TEXT("캐릭터가 없음! 캐릭터 생성 플로우 시작!!"));
		GI->ShowSlotSelectUI();
	}
	
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
