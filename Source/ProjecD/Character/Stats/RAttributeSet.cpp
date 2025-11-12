// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Stats/RAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

URAttributeSet::URAttributeSet()
{
	// 기본값(밸런싱 값은 데이터테이블로 분리 권장)
	Str = 10.f;
	Dex = 10.f;
	Con = 10.f;
	Int = 10.f;
	Win = 10.f;

	// 파생값 초기화
	AttackPower = 10.f;
	Defense = 0.f;
	MoveSpeed = 600.f;
	AttackSpeed = 1.f;
	MaxHealth = 100.f;
	HealthRegen = 1.f;
	MaxMana = 50.f;
	MagicPower = 5.f;
	ManaRegen = 0.5f;

	CurrentHealth = MaxHealth;
	CurrentMana = MaxMana;
}

void URAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URAttributeSet, Str, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URAttributeSet, Dex, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URAttributeSet, Con, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URAttributeSet, Int, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URAttributeSet, Win, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(URAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URAttributeSet, Defense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URAttributeSet, MagicPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URAttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(URAttributeSet, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URAttributeSet, CurrentMana, COND_None, REPNOTIFY_Always);
}

void URAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// MaxHealth/MaxMana에 영향이 가면 Current를 클램프(서버 권한에서 처리)
	if (Attribute == GetMaxHealthAttribute())
	{
		// optional: scale current health proportionally
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		// optional: scale current mana proportionally
	}
}

void URAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// 기본 속성(예: Str, Dex 등)이 변경되면 파생속성 재계산
	const FGameplayAttribute Primaries[] = {
		GetStrAttribute(), GetDexAttribute(), GetConAttribute(), GetIntAttribute(), GetWinAttribute()
	};

	for (const FGameplayAttribute& A : Primaries)
	{
		if (Data.EvaluatedData.Attribute == A)
		{
			RecalculateDerivedAttributes(&Data);
			break;
		}
	}

	// 예: 체력 감소에 따른 처리
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		// 죽음 등 처리(서버에서)
	}
}

void URAttributeSet::RecalculateDerivedAttributes(const FGameplayEffectModCallbackData* Data)
{
	// 간단한 계수(실제 값은 데이터테이블로 관리하세요)
	const float AttackPerStr = 2.0f;
	const float DefensePerStr = 1.0f;
	const float MoveSpeedPerDex = 2.0f; // 1 Dex -> +2 units move speed
	const float AttackSpeedPerDex = 0.01f; // 1 Dex -> +0.01 attack speed
	const float HealthPerCon = 10.0f;
	const float HealthRegenPerCon = 0.1f;
	const float ManaPerInt = 5.0f;
	const float MagicPowerPerInt = 1.5f;
	const float ManaRegenPerWin = 0.05f;

	// 읽기
	const float StrVal = Str.GetCurrentValue();
	const float DexVal = Dex.GetCurrentValue();
	const float ConVal = Con.GetCurrentValue();
	const float IntVal = Int.GetCurrentValue();
	const float WinVal = Win.GetCurrentValue();

	// 계산
	const float NewAttack = StrVal * AttackPerStr;
	const float NewDefense = StrVal * DefensePerStr;
	const float NewMoveSpeed = 600.f + DexVal * MoveSpeedPerDex;
	const float NewAttackSpeed = 1.f + DexVal * AttackSpeedPerDex;
	const float NewMaxHealth = 100.f + ConVal * HealthPerCon;
	const float NewHealthRegen = ConVal * HealthRegenPerCon;
	const float NewMaxMana = 50.f + IntVal * ManaPerInt;
	const float NewMagicPower = IntVal * MagicPowerPerInt;
	const float NewManaRegen = WinVal * ManaRegenPerWin;



	// GetOwningAbilitySystemComponent를 통해 SetNumericAttributeBase 호출 (AttributeSet 자체에는 SetNumericAttributeBase 멤버가 없음)
	if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
	{
		ASC->SetNumericAttributeBase(GetAttackPowerAttribute(), NewAttack);
		ASC->SetNumericAttributeBase(GetDefenseAttribute(), NewDefense);
		ASC->SetNumericAttributeBase(GetMoveSpeedAttribute(), NewMoveSpeed);
		ASC->SetNumericAttributeBase(GetAttackSpeedAttribute(), NewAttackSpeed);
		ASC->SetNumericAttributeBase(GetMaxHealthAttribute(), NewMaxHealth);
		ASC->SetNumericAttributeBase(GetHealthRegenAttribute(), NewHealthRegen);
		ASC->SetNumericAttributeBase(GetMaxManaAttribute(), NewMaxMana);
		ASC->SetNumericAttributeBase(GetMagicPowerAttribute(), NewMagicPower);
		ASC->SetNumericAttributeBase(GetManaRegenAttribute(), NewManaRegen);

		UE_LOG(LogTemp, Log, TEXT("[URAttributeSet] ASC->SetNumericAttributeBase(MoveSpeed) -> %f (ASC=%s)"), NewMoveSpeed, ASC ? *ASC->GetName() : TEXT("null"));

		// 현재 체력/마나는 최대치 변경에 맞춰 클램프
		if (CurrentHealth.GetCurrentValue() > NewMaxHealth)
		{
			ASC->SetNumericAttributeBase(GetCurrentHealthAttribute(), NewMaxHealth);
		}
		if (CurrentMana.GetCurrentValue() > NewMaxMana)
		{
			ASC->SetNumericAttributeBase(GetCurrentManaAttribute(), NewMaxMana);
		}
	}
	else
	{
		// AbilitySystemComponent가 없을 때의 안전한 폴백(서버/복제 시나리오에서는 ASC 사용 권장)
		AttackPower.SetBaseValue(NewAttack);
		Defense.SetBaseValue(NewDefense);
		MoveSpeed.SetBaseValue(NewMoveSpeed);
		AttackSpeed.SetBaseValue(NewAttackSpeed);
		MaxHealth.SetBaseValue(NewMaxHealth);
		HealthRegen.SetBaseValue(NewHealthRegen);
		MaxMana.SetBaseValue(NewMaxMana);
		MagicPower.SetBaseValue(NewMagicPower);
		ManaRegen.SetBaseValue(NewManaRegen);

		if (CurrentHealth.GetCurrentValue() > NewMaxHealth)
		{
			CurrentHealth.SetCurrentValue(NewMaxHealth);
		}
		if (CurrentMana.GetCurrentValue() > NewMaxMana)
		{
			CurrentMana.SetCurrentValue(NewMaxMana);
		}
	}
}
