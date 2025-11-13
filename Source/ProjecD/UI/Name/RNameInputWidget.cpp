#include "UI/Name/RNameInputWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Core/RGameInstance.h"
#include "Core/Subsystem/ROutGameCharacterDataSubsystem.h"
#include "UI/Manager/ROutGameUIManager.h"

void URNameInputWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	//-----버튼 바인딩------
	if (CreateButton)
	{
		CreateButton->OnClicked.Clear();
		CreateButton->OnClicked.AddDynamic(this,&URNameInputWidget::OnCreateClicked);
		UE_LOG(LogTemp,Log,TEXT("생성 버튼 바인딩 완료!!"));
	}
	
	if (CancelButton)
	{
		CancelButton->OnClicked.Clear();
		CancelButton->OnClicked.AddDynamic(this,&URNameInputWidget::OnCancelClicked);
		UE_LOG(LogTemp,Log,TEXT("취소 버튼 바인딩 완료!!"));
	}

	// 텍스트 입력 바인딩
	if (NameInputBox)
	{
		NameInputBox->OnTextChanged.Clear();
		NameInputBox->OnTextChanged.AddDynamic(this,&URNameInputWidget::OnNameTextChanged);
		NameInputBox->SetHintText(FText::FromString(TEXT("닉네임을 입력하세요")));
		UE_LOG(LogTemp,Log,TEXT("텍스트 입력 바인딩 완료!"));
	}

	//초기 에러 메시지 지우기
	if (ErrorMessageText)
	{
		ErrorMessageText->SetText(FText::GetEmpty());
	}

	// 닉네임 규칙 텍스트 설정
	if (RuleText)
	{
		RuleText->SetText(FText::FromString(TEXT("닉네임은 2~10자, 한글/영문/숫자만 가능")));
	}

	UE_LOG(LogTemp,Log,TEXT("RNameInputWidget 초기화 완료"));
	
}

void URNameInputWidget::OnNameTextChanged(const FText& Text)
{
	// 실시간 유효성 검사
	FString InputName=Text.ToString();
	ValidateAndShowError(InputName);
}

void URNameInputWidget::OnCreateClicked()
{
	if (!NameInputBox)
	{
		UE_LOG(LogTemp,Error,TEXT("NameInputBox가 nullptr!!"));
		return;
	}

	//입력된 닉네임을 가져오기
	FString InputName=NameInputBox->GetText().ToString();

	//마지막 유효성 검사
	if (!ValidateAndShowError(InputName))
	{
		UE_LOG(LogTemp,Warning,TEXT("유효하지않은 닉네임: %s"),*InputName);
		return;
	}

	//서브시스템 가져오기
	UROutGameCharacterDataSubsystem* Subsystem=URGameInstance::GetCharacterDataSubsystem(this);
	if (!Subsystem)
	{
		UE_LOG(LogTemp,Error,TEXT("CharacterDataSubSystem 접근 실패!!!"));
		return;
	}

	//캐릭터 생성
	bool bSuccess=Subsystem->CreateCharacter(
		Subsystem->SelectedCharacterIndex,
		Subsystem->TempSelectedClass,
		InputName
		);

	if (bSuccess)
	{
		UE_LOG(LogTemp,Log,TEXT("캐릭터 생성 성공!!"));
		UE_LOG(LogTemp,Log,TEXT("- 이름: %s"),*InputName);
		UE_LOG(LogTemp,Log,TEXT("- 직업: %d"),(int32)Subsystem->TempSelectedClass);
		UE_LOG(LogTemp,Log,TEXT("- 슬롯: %d"),Subsystem->SelectedCharacterIndex);

		//슬롯 선택 UI로 복귀
		URGameInstance* GI=GetGameInstance();
		if (GI)
		{
			GI->ShowSlotSelectUI();
			UE_LOG(LogTemp,Log,TEXT("슬롯 UI로 복귀 완료!"));
		}
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("캐릭터 생성 실패!!"));
		if (ErrorMessageText)
		{
			ErrorMessageText->SetText(FText::FromString(TEXT("캐릭터 생성에 실패했습니다!!")));
		}
	}
}

void URNameInputWidget::OnCancelClicked()
{
	UE_LOG(LogTemp,Log,TEXT("취소 버튼 클릭됨!!"));

	URGameInstance* GI=GetGameInstance();
	if (!GI) return;

	UROutGameUIManager* UIManager=GI->GetUIManager();
	if (UIManager)
	{
		UIManager->GoBackToPreviousUI();
		UE_LOG(LogTemp,Log,TEXT("이전 UI(직업 선택)으로 돌아가기!"));
	}
}

bool URNameInputWidget::ValidateAndShowError(const FString& Name)
{
	UROutGameCharacterDataSubsystem* Subsystem=URGameInstance::GetCharacterDataSubsystem(this);
	if (!Subsystem || !ErrorMessageText)
	{
		return false;
	}

	//유효성 검사
	FString ErrorMessage;
	bool bIsValid=Subsystem->IsValidCharacterName(Name,ErrorMessage);

	if (bIsValid)
	{
		//유효한 경우 - 에러 메시지 제거
		ErrorMessageText->SetText(FText::GetEmpty());
		
	}
	else
	{
		//유효하지 않은 경우- 에러 메시지 표시
		ErrorMessageText->SetText(FText::FromString(ErrorMessage));
	}

	return bIsValid;
}

URGameInstance* URNameInputWidget::GetGameInstance() const
{
	return Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}
