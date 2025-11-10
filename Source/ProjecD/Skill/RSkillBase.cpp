// 프로젝트 설정의 설명 페이지에 저작권 표시를 채우십시오.


#include "Skill/RSkillBase.h"
#include "Engine/DataTable.h" // FDataTableRowHandle에 필요

URSkillBase::URSkillBase()
{
    // 필요한 경우 기본 생성자 로직
	BonusDamage = 0.0f;
	AttackSpeedModifier = 1.0f;
}

void URSkillBase::PostInitProperties()
{
    Super::PostInitProperties();
    if (!HasAnyFlags(RF_ClassDefaultObject)) // CDO(클래스 기본 오브젝트)에서는 실행하지 않음
    {
        if (SkillDataHandle.DataTable && !SkillDataHandle.RowName.IsNone())
        {
            FRSkillData* Data = SkillDataHandle.GetRow<FRSkillData>(TEXT("SkillData를 찾을 수 없습니다."));
            if (Data)
            {
                CachedSkillData = *Data;
            }
        }
    }
}

void URSkillBase::InitializeSkill(const FRSkillData& InSkillData)
{
    CachedSkillData = InSkillData;
}

void URSkillBase::ActivateSkill_Implementation()
{
    // ActivateSkill의 기본 구현입니다.
    // 특정 스킬 클래스에서 이를 오버라이드합니다.
    UE_LOG(LogTemp, Warning, TEXT("기본 스킬에서 ActivateSkill_Implementation 호출됨: %s"), *CachedSkillData.SkillName.ToString());
}

void URSkillBase::ApplyPassiveEffect_Implementation()
{
    // ApplyPassiveEffect의 기본 구현입니다.
    // 특정 패시브 스킬 클래스에서 이를 오버라이드합니다.
    UE_LOG(LogTemp, Warning, TEXT("기본 스킬에서 ApplyPassiveEffect_Implementation 호출됨: %s"), *CachedSkillData.SkillName.ToString());
}

void URSkillBase::RemovePassiveEffect_Implementation()
{
    // RemovePassiveEffect의 기본 구현입니다.
    // 특정 패시브 스킬 클래스에서 이를 오버라이드합니다.
    UE_LOG(LogTemp, Warning, TEXT("기본 스킬에서 RemovePassiveEffect_Implementation 호출됨: %s"), *CachedSkillData.SkillName.ToString());
}

float URSkillBase::GetTotalDamage() const
{
	return CachedSkillData.BaseDamage + BonusDamage;
}

float URSkillBase::GetFinalCooldown() const
{
	if (AttackSpeedModifier <= 0.0f)
	{
		return CachedSkillData.Cooldown;
	}
	return CachedSkillData.Cooldown / AttackSpeedModifier;
}