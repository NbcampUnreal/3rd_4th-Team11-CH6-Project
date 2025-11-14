// Fill out your copyright notice in the Description page of Project Settings.

#include "RBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Character/Stats/RAttributeSet.h"
#include "EnhancedInputComponent.h"
#include "Controller/RPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Abilities/GameplayAbility.h"


ARBaseCharacter::ARBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	// Ability System Component 생성 및 복제 설정
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetIsReplicated(true);
		// 필요시 복제 모드 설정: AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	}

	// AttributeSet 생성 및 ASC에 등록 (ASC가 먼저 있어야 등록 가능)
	AttributeSet = CreateDefaultSubobject<URAttributeSet>(TEXT("AttributeSet"));
	if (AbilitySystemComponent && AttributeSet)
	{
		AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet);
	}

	/*SkillSlotAbilities.SetNumZeroed(Skillslots);
	SkillSlotAbilityHandles.SetNumZeroed(Skillslots);*/
}

// Called every frame
void ARBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 서버에서만 재생 적용
	if (HasAuthority() && AttributeSet && AbilitySystemComponent)
	{
		// Health regen
		const float MaxHP = AttributeSet->MaxHealth.GetCurrentValue();
		float CurrHP = AttributeSet->CurrentHealth.GetCurrentValue();
		const float RegenHP = AttributeSet->HealthRegen.GetCurrentValue();

		if (RegenHP > 0.f && CurrHP < MaxHP)
		{
			CurrHP = FMath::Min(MaxHP, CurrHP + RegenHP * DeltaTime);
			// Apply change on server
			AbilitySystemComponent->SetNumericAttributeBase(URAttributeSet::GetCurrentHealthAttribute(), CurrHP);
			UE_LOG(LogTemp, Verbose, TEXT("[ARBaseCharacter] Tick applied HealthRegen -> %f"), CurrHP);
		}

		// Mana regen
		const float MaxM = AttributeSet->MaxMana.GetCurrentValue();
		float CurrM = AttributeSet->CurrentMana.GetCurrentValue();
		const float RegenM = AttributeSet->ManaRegen.GetCurrentValue();

		if (RegenM > 0.f && CurrM < MaxM)
		{
			CurrM = FMath::Min(MaxM, CurrM + RegenM * DeltaTime);
			AbilitySystemComponent->SetNumericAttributeBase(URAttributeSet::GetCurrentManaAttribute(), CurrM);
			UE_LOG(LogTemp, Verbose, TEXT("[ARBaseCharacter] Tick applied ManaRegen -> %f"), CurrM);
		}
	}
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
			if (PC->SkillInputActions[0])
			{
				EhancedInput->BindAction(PC->SkillInputActions[0], ETriggerEvent::Triggered, this, &ARBaseCharacter::OnSkillSlot1);
			}
			if (PC->SkillInputActions[1])
			{
				EhancedInput->BindAction(PC->SkillInputActions[1], ETriggerEvent::Triggered, this, &ARBaseCharacter::OnSkillSlot2);
			}
			if (PC->SkillInputActions[2])
			{
				EhancedInput->BindAction(PC->SkillInputActions[2], ETriggerEvent::Triggered, this, &ARBaseCharacter::OnSkillSlot3);
			}
		}
	}

}

UAbilitySystemComponent* ARBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] PossessedBy called on %s (Authority=%d)"), *GetName(), HasAuthority() ? 1 : 0);
	UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] Before Init: ASC=%s AttributeSet=%s"),
		AbilitySystemComponent ? *AbilitySystemComponent->GetName() : TEXT("null"),
		AttributeSet ? *AttributeSet->GetName() : TEXT("null"));

	// AbilitySystem 초기화 및 서버에서 기본 Attribute 적용
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] AbilitySystemComponent initialized"));

		RegisterAttributeDelegates();

		// apply default attributes on server
		if (HasAuthority() && DefaultAttributeEffect)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1.0f, EffectContext);
			if (SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle H = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] Applied DefaultAttributeEffect, Handle valid=%d"), H.IsValid() ? 1 : 0);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[ARBaseCharacter] SpecHandle invalid for DefaultAttributeEffect"));
			}
		}

		// 초기값 적용(클라이언트/서버 모두)
		if (AttributeSet)
		{
			if (GetCharacterMovement())
			{
				GetCharacterMovement()->MaxWalkSpeed = AttributeSet->MoveSpeed.GetCurrentValue();
			}

			// cache derived values
			CurrentAttackPower = AttributeSet->AttackPower.GetCurrentValue();
			CurrentDefense = AttributeSet->Defense.GetCurrentValue();
			CurrentAttackSpeed = AttributeSet->AttackSpeed.GetCurrentValue();
			CurrentMagicPower = AttributeSet->MagicPower.GetCurrentValue();
			CurrentHealthRegen = AttributeSet->HealthRegen.GetCurrentValue();
			CurrentManaRegen = AttributeSet->ManaRegen.GetCurrentValue();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ARBaseCharacter] AbilitySystemComponent is null in PossessedBy"));
	}

	// 로그: AttributeSet 및 주요 값들 출력 (가능하면)
	if (AttributeSet)
	{
		UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] Attributes after Possess -> Str=%f Dex=%f MaxHP=%f CurrentHP=%f"),
			AttributeSet->Str.GetCurrentValue(),
			AttributeSet->Dex.GetCurrentValue(),
			AttributeSet->MaxHealth.GetCurrentValue(),
			AttributeSet->CurrentHealth.GetCurrentValue());
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ARBaseCharacter] AttributeSet is null after Possess"));
	}

	if (ARPlayerController* PC = Cast<ARPlayerController>(NewController))
	{
		PC->EnterGameControlMode();
	}
}

void ARBaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] OnRep_PlayerState called on %s (Authority=%d)"), *GetName(), HasAuthority() ? 1 : 0);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		RegisterAttributeDelegates();

		if (AttributeSet)
		{
			if (GetCharacterMovement())
			{
				GetCharacterMovement()->MaxWalkSpeed = AttributeSet->MoveSpeed.GetCurrentValue();
			}

			// cache derived values
			CurrentAttackPower = AttributeSet->AttackPower.GetCurrentValue();
			CurrentDefense = AttributeSet->Defense.GetCurrentValue();
			CurrentAttackSpeed = AttributeSet->AttackSpeed.GetCurrentValue();
			CurrentMagicPower = AttributeSet->MagicPower.GetCurrentValue();
			CurrentHealthRegen = AttributeSet->HealthRegen.GetCurrentValue();
			CurrentManaRegen = AttributeSet->ManaRegen.GetCurrentValue();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ARBaseCharacter] AbilitySystemComponent is null in OnRep_PlayerState"));
	}
}

// 입력 값 처리
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

void ARBaseCharacter::RegisterAttributeDelegates()
{
	if (!AbilitySystemComponent || bAttributeDelegatesRegistered) return;

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URAttributeSet::GetMoveSpeedAttribute())
		.AddUObject(this, &ARBaseCharacter::OnMoveSpeedChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URAttributeSet::GetAttackPowerAttribute())
		.AddUObject(this, &ARBaseCharacter::OnAttackPowerChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URAttributeSet::GetDefenseAttribute())
		.AddUObject(this, &ARBaseCharacter::OnDefenseChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URAttributeSet::GetAttackSpeedAttribute())
		.AddUObject(this, &ARBaseCharacter::OnAttackSpeedChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URAttributeSet::GetMaxHealthAttribute())
		.AddUObject(this, &ARBaseCharacter::OnMaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URAttributeSet::GetHealthRegenAttribute())
		.AddUObject(this, &ARBaseCharacter::OnHealthRegenChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URAttributeSet::GetMaxManaAttribute())
		.AddUObject(this, &ARBaseCharacter::OnMaxManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URAttributeSet::GetMagicPowerAttribute())
		.AddUObject(this, &ARBaseCharacter::OnMagicPowerChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URAttributeSet::GetManaRegenAttribute())
		.AddUObject(this, &ARBaseCharacter::OnManaRegenChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URAttributeSet::GetCurrentHealthAttribute())
		.AddUObject(this, &ARBaseCharacter::OnCurrentHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URAttributeSet::GetCurrentManaAttribute())
		.AddUObject(this, &ARBaseCharacter::OnCurrentManaChanged);

	bAttributeDelegatesRegistered = true;
}

void ARBaseCharacter::BindSkillAbility(int32 SlotIndex, TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (SlotIndex < 0 || SlotIndex >= Skillslots) return;

	// 동일 바인딩이면 아무 작업 안 함
	if (SkillSlotAbilities.IsValidIndex(SlotIndex) && SkillSlotAbilities[SlotIndex] == AbilityClass) return;

	// 이전 바인딩된 AbilityClass 및 Handle 저장
	TSubclassOf<UGameplayAbility> PrevClass;
	FGameplayAbilitySpecHandle PrevHandle;
	if (SkillSlotAbilities.IsValidIndex(SlotIndex))
	{
		PrevClass = SkillSlotAbilities[SlotIndex];
	}
	if (SkillSlotAbilityHandles.IsValidIndex(SlotIndex))
	{
		PrevHandle = SkillSlotAbilityHandles[SlotIndex];
	}

	// 새 AbilityClass로 슬롯 갱신 (nullptr 허용 -> 슬롯 언바인드 의미)
	SkillSlotAbilities[SlotIndex] = AbilityClass;

	// 서버에서만 ASC에 실제 Give/Remove 수행 (권한 중요)
	if (!AbilitySystemComponent) return;

	// 이전에 부여했던 Ability가 있으면 제거 시도
	if (PrevHandle.IsValid())
	{
		// ClearAbility는 프로젝트의 GAS 버전에 따라 함수명이 다를 수 있습니다.
		// 대부분의 프로젝트에서는 Handle을 이용해 제거하는 API를 제공합니다.
		// 존재하지 않으면 프로젝트에 맞춰 적절한 제거 API로 바꿔 주세요.
		AbilitySystemComponent->ClearAbility(PrevHandle);
		SkillSlotAbilityHandles[SlotIndex] = FGameplayAbilitySpecHandle(); // reset
		UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] Cleared previous ability from slot %d"), SlotIndex);
	}

	// 새로 바인딩할 Ability가 nullptr이면 언바인드만 수행
	if (!AbilityClass)
	{
		UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] Slot %d unbound"), SlotIndex);
		return;
	}

	// 서버 권한이 있는 경우 새 Ability 부여
	if (HasAuthority())
	{
		FGameplayAbilitySpec Spec(AbilityClass, 1, INDEX_NONE, this);
		FGameplayAbilitySpecHandle NewHandle = AbilitySystemComponent->GiveAbility(Spec);
		SkillSlotAbilityHandles[SlotIndex] = NewHandle;

		UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] Bound Ability %s to slot %d (Handle valid=%d)"),
			*AbilityClass->GetName(), SlotIndex, NewHandle.IsValid() ? 1 : 0);
	}
	else
	{
		// 클라이언트에서 호출될 경우, 서버 RPC로 요청해야 합니다.
		UE_LOG(LogTemp, Warning, TEXT("[ARBaseCharacter] BindSkillAbility called on client for slot %d. Call via server RPC."), SlotIndex);
	}
}

// callbacks
void ARBaseCharacter::OnMoveSpeedChanged(const FOnAttributeChangeData& Data)
{
	const float NewVal = Data.NewValue;
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = NewVal;
		UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] MoveSpeed changed -> %f applied"), NewVal);
	}
}

void ARBaseCharacter::OnAttackPowerChanged(const FOnAttributeChangeData& Data)
{
	CurrentAttackPower = Data.NewValue;
	UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] AttackPower changed -> %f"), CurrentAttackPower);
	// 사용: 공격계산에서 CurrentAttackPower 참조
}

void ARBaseCharacter::OnDefenseChanged(const FOnAttributeChangeData& Data)
{
	CurrentDefense = Data.NewValue;
	UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] Defense changed -> %f"), CurrentDefense);
	// 사용: 피해 계산에서 CurrentDefense 참조
}

void ARBaseCharacter::OnAttackSpeedChanged(const FOnAttributeChangeData& Data)
{
	CurrentAttackSpeed = Data.NewValue;
	UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] AttackSpeed changed -> %f"), CurrentAttackSpeed);
	// 사용: 공격 쿨다운/애니메이션 재생속도에 적용
}

void ARBaseCharacter::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	const float NewMax = Data.NewValue;
	UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] MaxHealth changed -> %f"), NewMax);

	// 서버에서 CurrentHealth를 Max로 클램프/스케일
	if (HasAuthority() && AttributeSet && AbilitySystemComponent)
	{
		float Curr = AttributeSet->CurrentHealth.GetCurrentValue();
		if (Curr > NewMax)
		{
			AbilitySystemComponent->SetNumericAttributeBase(URAttributeSet::GetCurrentHealthAttribute(), NewMax);
		}
	}
}

void ARBaseCharacter::OnHealthRegenChanged(const FOnAttributeChangeData& Data)
{
	CurrentHealthRegen = Data.NewValue;
	UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] HealthRegen changed -> %f"), CurrentHealthRegen);
}

void ARBaseCharacter::OnMaxManaChanged(const FOnAttributeChangeData& Data)
{
	const float NewMax = Data.NewValue;
	UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] MaxMana changed -> %f"), NewMax);

	if (HasAuthority() && AttributeSet && AbilitySystemComponent)
	{
		float Curr = AttributeSet->CurrentMana.GetCurrentValue();
		if (Curr > NewMax)
		{
			AbilitySystemComponent->SetNumericAttributeBase(URAttributeSet::GetCurrentManaAttribute(), NewMax);
		}
	}
}

void ARBaseCharacter::OnMagicPowerChanged(const FOnAttributeChangeData& Data)
{
	CurrentMagicPower = Data.NewValue;
	UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] MagicPower changed -> %f"), CurrentMagicPower);
}

void ARBaseCharacter::OnManaRegenChanged(const FOnAttributeChangeData& Data)
{
	CurrentManaRegen = Data.NewValue;
	UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] ManaRegen changed -> %f"), CurrentManaRegen);
}

void ARBaseCharacter::OnCurrentHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] CurrentHealth changed -> %f"), Data.NewValue);
	// UI 업데이트 등 여기서 처리
}

void ARBaseCharacter::OnCurrentManaChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] CurrentMana changed -> %f"), Data.NewValue);
	// UI 업데이트 등 여기서 처리
}

void ARBaseCharacter::ActivateSkillInSlot(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= Skillslots) return;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ARBaseCharacter] No AbilitySystemComponent when activating slot %d"), SlotIndex);
		return;
	}

	// 슬롯에 바인딩된 Ability 클래스 확인
	TSubclassOf<UGameplayAbility> SkillClass = nullptr;
	if (SkillSlotAbilities.IsValidIndex(SlotIndex))
	{
		SkillClass = SkillSlotAbilities[SlotIndex];
	}

	if (!SkillClass)
	{
		UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] No ability bound to slot %d"), SlotIndex);
		return;
	}

	// 1) 가능한 경우 저장된 SpecHandle로 시도 (서버에서 GiveAbility 했을 때 유효)
	if (SkillSlotAbilityHandles.IsValidIndex(SlotIndex) && SkillSlotAbilityHandles[SlotIndex].IsValid())
	{
		const FGameplayAbilitySpecHandle& Handle = SkillSlotAbilityHandles[SlotIndex];
		bool bActivatedByHandle = AbilitySystemComponent->TryActivateAbility(Handle);
		if (bActivatedByHandle)
		{
			UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] Activated ability (handle) %s in slot %d"), *SkillClass->GetName(), SlotIndex);
			return;
		}
		// 실패하면 폴백으로 클래스 기반 시도
	}

	// 2) 클래스 기반 활성화 시도 (클라이언트/서버 모두 가능, 단 해당 클래스가 ASC에 부여되어 있어야 활성화됨)
	bool bActivatedByClass = AbilitySystemComponent->TryActivateAbilityByClass(SkillClass);
	if (bActivatedByClass)
	{
		UE_LOG(LogTemp, Log, TEXT("[ARBaseCharacter] Activated ability (class) %s in slot %d"), *SkillClass->GetName(), SlotIndex);
		return;
	}

	// 3) 활성화 실패 처리: 보통 클라이언트에서 호출했을 때 서버에 ability가 없어서 실패하는 경우가 많음.
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ARBaseCharacter] Activation failed for %s in slot %d on client. Request server activation via RPC."),
			*SkillClass->GetName(), SlotIndex);
		// 권장: ARPlayerController 또는 캐릭터에 Server RPC를 만들어 서버에서 Activate 요청을 수행하세요.
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ARBaseCharacter] Activation failed for %s in slot %d on server. Check if ability was granted correctly."),
			*SkillClass->GetName(), SlotIndex);
	}
}
// 콜백 함수
void ARBaseCharacter::OnSkillSlot1(const FInputActionValue& Value) { ActivateSkillInSlot(0); }
void ARBaseCharacter::OnSkillSlot2(const FInputActionValue& Value) { ActivateSkillInSlot(1); }
void ARBaseCharacter::OnSkillSlot3(const FInputActionValue& Value) { ActivateSkillInSlot(2); }