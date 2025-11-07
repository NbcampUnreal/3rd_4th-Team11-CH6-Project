#include "UI/Slot/RSlotSelectWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Core/RGameInstance.h"

void URSlotSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//버튼 바인딩
	if (Slot0Button) Slot0Button->OnClicked.AddDynamic(this,&URSlotSelectWidget::OnSlot0Clicked);
	if (Slot1Button) Slot1Button->OnClicked.AddDynamic(this,&URSlotSelectWidget::OnSlot1Clicked);
	if (Slot2Button) Slot2Button->OnClicked.AddDynamic(this,&URSlotSelectWidget::OnSlot2Clicked);

	RefreshSlots();
}

void URSlotSelectWidget::RefreshSlots()
{
	UpdateSlotUI(0,Slot0Button,Slot0Name,Slot0Class);
	UpdateSlotUI(1,Slot1Button,Slot1Name,Slot1Class);
	UpdateSlotUI(2,Slot2Button,Slot2Name,Slot2Class);
}

void URSlotSelectWidget::OnSlot0Clicked() {HandleSlotClick(0);}
void URSlotSelectWidget::OnSlot1Clicked() {HandleSlotClick(1);}
void URSlotSelectWidget::OnSlot2Clicked() {HandleSlotClick(2); }


void URSlotSelectWidget::HandleSlotClick(int32 SlotIndex)
{
	URGameInstance* GI=GetGameInstance();
	if (!GI) return;

	if (GI->IsSlotEmpty(SlotIndex))
	{
		// 빈 슬롯 -> 대상 슬롯 저장후 직업 선택 UI로 이동
		GI->SelectedCharacterIndex=SlotIndex; // 직업 선택 이후 해당 슬롯에 생성예정
		UE_LOG(LogTemp,Warning,TEXT("빈 슬롯 %d 클릭->직업 선택 UI로 이동(미구현)"));

		// 구현후 주석해제 예정
		// GI->GetUIManager()->ShowUI(GI->JobSelectWidgetClass);
	}
	else
	{
		//생성된 캐릭터 -> 선택
		if (GI->SelectCharacter(SlotIndex))
		{
			UE_LOG(LogTemp,Log,TEXT("캐릭터 선택 완료! 슬롯 %d"),SlotIndex);
			// 게임월드로 이동하는것 구현예정
		}
	}
}

void URSlotSelectWidget::UpdateSlotUI(int32 SlotIndex, UButton* Button, UTextBlock* NameText, UTextBlock* ClassText)
{
	URGameInstance* GI=GetGameInstance();
	if (!GI) return;

	FCharacterSlotData Data=GI->GetCharacterData(SlotIndex);

	if (Data.bIsCreated)
	{
		//생성된 캐릭터 표시
		if (Button) Button->SetIsEnabled(true);
		if (NameText) NameText->SetText(FText::FromString(Data.CharacterName));

		FString ClassName;
		switch (Data.CharacterClass)
		{
		case ECharacterClassType::Knight: ClassName=TEXT("KNIGHT"); break;
		case ECharacterClassType::Archer: ClassName=TEXT("ARCHER"); break;
		case ECharacterClassType::Mage:   ClassName=TEXT("Mage"); break;
		default: ClassName=TEXT("ERROR"); break;
		}
		if (ClassText) ClassText->SetText(FText::FromString(ClassName));
	}
	else
	{
		// 빈 슬롯 표시
		if (Button) Button->SetIsEnabled(true);
		if (NameText) NameText->SetText(FText::FromString("새 캐릭터\n생성"));
		if (ClassText) ClassText->SetText(FText::GetEmpty());
	}
}

URGameInstance* URSlotSelectWidget::GetGameInstance() const
{
	return Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}
