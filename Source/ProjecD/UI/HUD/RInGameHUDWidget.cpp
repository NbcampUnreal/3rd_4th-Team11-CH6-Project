// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/RInGameHUDWidget.h"
#include "Components/TextBlock.h"
#include "Core/RGameInstance.h"
#include "Core/Subsystem/ROutGameCharacterDataSubsystem.h"

void URInGameHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!PlayerNameText)
	{
		UE_LOG(LogTemp, Warning, TEXT("RInGameHUDWidget: PlayerNameText 바인딩 안됨"));
		return;
	}

	if (UROutGameCharacterDataSubsystem* Subsystem = URGameInstance::GetCharacterDataSubsystem(this))
	{
		const FCharacterSlotData SelectedData = Subsystem->GetSelectedCharacterData();
		PlayerNameText->SetText(FText::FromString(SelectedData.CharacterName));

		UE_LOG(LogTemp, Log, TEXT("[RInGameHUDWidget] 이름: %s"), *SelectedData.CharacterName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("RInGameHUDWidget: CharacterDataSubsystem 못찾음"));
	}
}
