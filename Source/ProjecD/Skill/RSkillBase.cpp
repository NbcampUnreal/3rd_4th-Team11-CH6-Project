
#include "RSkillBase.h"
#include "Engine/DataTable.h" // FDataTableRowHandle에 필요

URSkillBase::URSkillBase()
{
    // 이 어빌리티는 캐릭터마다 인스턴스화되어야 상태(레벨, 버프 등)를 가질 수 있습니다.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

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