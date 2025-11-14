// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RLobbyHUDWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECD_API URLobbyHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	// WBP_LobbyHUD 안 TextBlock 이름 = PlayerNameText
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;
};
