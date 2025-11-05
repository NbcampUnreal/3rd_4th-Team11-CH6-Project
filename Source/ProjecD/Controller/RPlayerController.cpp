// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/RPlayerController.h"
#include "EnhancedInputSubsystems.h"

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