// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/RPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

ARPlayerController::ARPlayerController() : DefaultMappingContext(nullptr), MoveAction(nullptr), LookAction(nullptr), JumpAction(nullptr)
{
	
}

void ARPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if(ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if(DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext,0);
			}
		}
	}
}

void ARPlayerController::EnterGameControlMode()
{
	if (!IsLocalPlayerController()) return;

	bShowMouseCursor = false;

	FInputModeGameOnly Mode;
	SetInputMode(Mode);

	UE_LOG(LogTemp,Warning,TEXT("[ARPlayerController] 게임만 커서 OFF"));
}

void ARPlayerController::EnterUIMode(UUserWidget* FocusWidget)
{
	if (!IsLocalController()) return;

	bShowMouseCursor = true;

	FInputModeGameAndUI Mode;
	if (FocusWidget)
	{
		Mode.SetWidgetToFocus(FocusWidget->TakeWidget());
	}
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(Mode);

	UE_LOG(LogTemp, Log, TEXT("[ARPlayerController] EnterUIMode: GameAndUI + Cursor ON"));
}