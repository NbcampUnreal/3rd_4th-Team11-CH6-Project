// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Controller/RPlayerController.h"

// Sets default values
ARBaseCharacter::ARBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;
}

// Called every frame
void ARBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		ARPlayerController* PC = Cast<ARPlayerController>(GetController());
		if (PC)
		{
			if (PC->MoveAction)
			{
				EhancedInput->BindAction(PC->MoveAction, ETriggerEvent::Triggered, this, &ARBaseCharacter::Move);
			}
			if (PC->LookAction)
			{
				EhancedInput->BindAction(PC->LookAction, ETriggerEvent::Triggered, this, &ARBaseCharacter::Look);
			}
			if (PC->JumpAction)
			{
				EhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Triggered, this, &ARBaseCharacter::StartJump);
				EhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Completed, this, &ARBaseCharacter::StopJump);
			}
		}
	}

}
void ARBaseCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	FVector2D MoveInput = Value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}
void ARBaseCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}
void ARBaseCharacter::StartJump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		Jump();
	}
}
void ARBaseCharacter::StopJump(const FInputActionValue& Value)
{
	if (!Value.Get<bool>())
	{
		StopJumping();
	}
}
