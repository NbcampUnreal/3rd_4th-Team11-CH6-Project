#include "UI/Slot/RSlotSelectWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Core/RGameInstance.h"
#include "Core/Subsystem/ROutGameCharacterDataSubsystem.h"

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

	UROutGameCharacterDataSubsystem* Subsystem=URGameInstance::GetCharacterDataSubsystem(this);
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterDataSubsystem 접근 실패!"));
		return;
	}
	
	if (Subsystem->IsSlotEmpty(SlotIndex))
	{
		// 빈 슬롯 -> 대상 슬롯 저장후 직업 선택 UI로 이동
		Subsystem->SelectedCharacterIndex=SlotIndex;
		UE_LOG(LogTemp, Warning, TEXT("빈 슬롯 클릭->직업 선택 UI로 이동(미구현)"));

		GI->ShowClassSelectUI();
	}
	else
	{
		//생성된 캐릭터 -> 선택
		if (Subsystem->SelectCharacter(SlotIndex)) 
		{
			UE_LOG(LogTemp,Log,TEXT("캐릭터 선택 완료! 슬롯 %d"),SlotIndex); // 게임월드로 이동하는것 구현예정
		}
	}
	

}

void URSlotSelectWidget::UpdateSlotUI(int32 SlotIndex, UButton* Button, UTextBlock* NameText, UTextBlock* ClassText)
{
	UROutGameCharacterDataSubsystem* Subsystem=URGameInstance::GetCharacterDataSubsystem(this);
	if (!Subsystem) return;

	FCharacterSlotData Data=Subsystem->GetCharacterData(SlotIndex);

	//Border 배열
	UBorder* Borders[]= { Slot0Border,Slot1Border,Slot2Border};
	UBorder* CurrentBorder=(SlotIndex >= 0 && SlotIndex < 3) ? Borders[SlotIndex] : nullptr;

	if (Data.bIsCreated)
	{
		//생성된 캐릭터 표시
		if (Button) Button->SetIsEnabled(true);
		if (NameText) NameText->SetText(FText::FromString(Data.CharacterName));

		FString ClassName;
		switch (Data.CharacterClass)
		{
		case ECharacterClassType::Knight: ClassName="KNIGHT"; break;
		case ECharacterClassType::Archer: ClassName="ARCHER"; break;
		case ECharacterClassType::Mage:   ClassName="Mage"; break;
		default: ClassName=TEXT("ERROR"); break;
		}
		if (ClassText) ClassText->SetText(FText::FromString(ClassName));

		if (CurrentBorder) CurrentBorder->SetBrushColor(FLinearColor(0.20f, 0.60f, 0.86f, 1.0f)); // #3498DB
	}
	else
	{
		// 빈 슬롯 표시
		if (Button) Button->SetIsEnabled(true);
		if (NameText) NameText->SetText(FText::FromString("Create New Character"));
		if (ClassText) ClassText->SetText(FText::FromString("+"));

		if (CurrentBorder) CurrentBorder->SetBrushColor(FLinearColor(0.17f, 0.24f, 0.31f, 1.0f)); // #2C3E50
	}
	
}

URGameInstance* URSlotSelectWidget::GetGameInstance() const
{
	return Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}
