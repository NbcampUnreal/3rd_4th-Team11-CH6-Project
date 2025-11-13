#include "UI/Class/RClassSelectWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Core/RGameInstance.h"
#include "UI/Manager/ROutGameUIManager.h"
#include "Core/Subsystem/ROutGameCharacterDataSubsystem.h"


void URClassSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//버튼 클릭 이벤트 바인딩
	if (KnightButton)
	{
		KnightButton->OnClicked.Clear();
		KnightButton->OnClicked.AddDynamic(this,&URClassSelectWidget::OnKnightButtonClicked);
	}
	if (ArcherButton)
	{
		ArcherButton->OnClicked.Clear();
		ArcherButton->OnClicked.AddDynamic(this,&URClassSelectWidget::OnArcherButtonClicked);
	}
	if (MageButton)
	{
		MageButton->OnClicked.Clear();
		MageButton->OnClicked.AddDynamic(this,&URClassSelectWidget::OnMageButtonClicked);
	}

	//직업명 설정 (고정됨!)
	if (KnightNameText) KnightNameText->SetText(FText::FromString(TEXT("KNIGHT")));
	if (ArcherNameText) ArcherNameText->SetText(FText::FromString(TEXT("ARCHER")));
	if (MageNameText) MageNameText->SetText(FText::FromString(TEXT("MAGE")));

	//버튼 상태 갱신
	RefreshClassButtons();
	UE_LOG(LogTemp,Log,TEXT("RClassSelectWidget 초기화 완료!!"));
	
}

void URClassSelectWidget::RefreshClassButtons()
{
	//각 직업별 버튼 상태 업데이트
	UpdateClassButton(KnightButton,KnightStatusText,ECharacterClassType::Knight);
	UpdateClassButton(ArcherButton,ArcherStatusText,ECharacterClassType::Archer);
	UpdateClassButton(MageButton,MageStatusText,ECharacterClassType::Mage);
}

void URClassSelectWidget::UpdateClassButton(UButton* Button, UTextBlock* StatusText, ECharacterClassType Class)
{
	if (!Button || !StatusText) return;

	// Border 설정
	UBorder* CurrentBorder=nullptr;
	if (Class == ECharacterClassType::Knight) CurrentBorder=KnightBorder;
	else if (Class == ECharacterClassType::Archer) CurrentBorder=ArcherBorder;
	else if (Class == ECharacterClassType::Mage) CurrentBorder=MageBorder;
	
	if (IsClassAlreadyCreated(Class))
	{
		//이미 생성된 직업-> 비활성화!!
		Button->SetIsEnabled(false);
		StatusText->SetText(FText::FromString(TEXT("Created")));

		if (CurrentBorder)
		{
			CurrentBorder->SetBrushColor(FLinearColor(0.15f, 0.20f, 0.25f, 1.0f));
		}

		UE_LOG(LogTemp,Log,TEXT("직업 %d 이미 생성됨 - 비활성화!"),(int32)Class);
	}
	else
	{
		//선택가능->활성화
		Button->SetIsEnabled(true);
		StatusText->SetText(FText::FromString(TEXT("Choose")));

		if (CurrentBorder)
		{
			CurrentBorder->SetBrushColor(FLinearColor(0.70f, 0.70f, 0.70f, 1.0f));
		}
	}
}

bool URClassSelectWidget::IsClassAlreadyCreated(ECharacterClassType Class) const
{
	UROutGameCharacterDataSubsystem* Subsystem=URGameInstance::GetCharacterDataSubsystem(this);
	if (!Subsystem) return false;

	//모든 슬롯 확인
	for (const FCharacterSlotData& SlotData: Subsystem->CharacterSlots)
	{
		if (SlotData.bIsCreated && SlotData.CharacterClass==Class)
		{
			return true; // 이미 생성된 직업을 발견한 경우
		}
	}

	return false; // 생성 안함
}


void URClassSelectWidget::OnKnightButtonClicked()
{
	UE_LOG(LogTemp,Log,TEXT("Knight 선택됨!"));
	HandleClassSelection(ECharacterClassType::Knight);
}

void URClassSelectWidget::OnArcherButtonClicked()
{
	UE_LOG(LogTemp,Log,TEXT("Archer 선택됨!"));
	HandleClassSelection(ECharacterClassType::Archer);
}

void URClassSelectWidget::OnMageButtonClicked()
{
	UE_LOG(LogTemp,Log,TEXT("Mage 선택됨!"));
	HandleClassSelection(ECharacterClassType::Mage);
}

void URClassSelectWidget::HandleClassSelection(ECharacterClassType SelectedClass)
{
	UROutGameCharacterDataSubsystem* Subsystem=URGameInstance::GetCharacterDataSubsystem(this);
	if (!Subsystem)
	{
		UE_LOG(LogTemp,Error,TEXT("GameInstance가 Nullptr!!"));
		return;
	}

	//선택한 직업 임시 저장하기
	Subsystem->TempSelectedClass=SelectedClass;
	UE_LOG(LogTemp, Log, TEXT("직업 선택 완료! Class: %d, 목표 슬롯: %d"), (int32)SelectedClass, Subsystem->SelectedCharacterIndex);

	//이릅 입력 UI(NameInputUI)로 전환
	URGameInstance* GI=GetGameInstance();
	if (GI && GI->GetUIManager())
	{
		GI->ShowNameInputUI();
		UE_LOG(LogTemp,Warning,TEXT("이름 입력 UI로 전환 요청됨"));
	}
}

URGameInstance* URClassSelectWidget::GetGameInstance() const
{
	return Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}