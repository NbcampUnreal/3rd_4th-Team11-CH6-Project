// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "RBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class URAttributeSet;
class UGameplayEffect;
struct FOnAttributeChangeData;

UCLASS()
class PROJECD_API ARBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARBaseCharacter();

	// Tick
	virtual void Tick(float DeltaTime) override;

	// Input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	// Movement / Input callbacks
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);

	// Ability System
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	URAttributeSet* AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Lifecycle
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Attribute change callbacks (일반 멤버 함수로 선언 — UFUNCTION 제거)
	void OnMoveSpeedChanged(const FOnAttributeChangeData& Data);
	void OnAttackPowerChanged(const FOnAttributeChangeData& Data);
	void OnDefenseChanged(const FOnAttributeChangeData& Data);
	void OnAttackSpeedChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	void OnHealthRegenChanged(const FOnAttributeChangeData& Data);
	void OnMaxManaChanged(const FOnAttributeChangeData& Data);
	void OnMagicPowerChanged(const FOnAttributeChangeData& Data);
	void OnManaRegenChanged(const FOnAttributeChangeData& Data);
	void OnCurrentHealthChanged(const FOnAttributeChangeData& Data);
	void OnCurrentManaChanged(const FOnAttributeChangeData& Data);

protected:
	// Prevent duplicate registrations
	bool bAttributeDelegatesRegistered = false;

	// Cached gameplay values for easy access by other systems
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentAttackPower = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentDefense = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentAttackSpeed = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentMagicPower = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentHealthRegen = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentManaRegen = 0.f;

	// Helper to register attribute delegates (called from PossessedBy / OnRep_PlayerState)
	void RegisterAttributeDelegates();
};
