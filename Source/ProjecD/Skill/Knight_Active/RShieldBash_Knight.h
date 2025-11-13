// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/RSkillBase.h"
#include "Animation/AnimMontage.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "GameplayEffectTypes.h" // TSubclassOf<UGameplayEffect>를 위해 필요
#include "RShieldBash_Knight.generated.h"

/**
 * 
 */
UCLASS()
class PROJECD_API URShieldBash_Knight : public URSkillBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShieldBash")
	float Damage = 20.0f; // 스킬 피해량

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShieldBash")
	float Range = 500.0f; // 부채꼴 범위 길이

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShieldBash")
	float Angle = 60.0f; // 부채꼴 범위 각도

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShieldBash")
	float StunDuration = 2.0f; // 기절 지속 시간

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShieldBash")
	UAnimMontage* ShieldBashMontage; // 재생할 애니메이션 몽타주

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShieldBash")
	UParticleSystem* ShieldBashVFX; // 재생할 시각 효과

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShieldBash")
	USoundCue* ShieldBashSound; // 재생할 사운드

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShieldBash")
	TSubclassOf<UGameplayEffect> StunGameplayEffectClass; // 기절 GameplayEffect 클래스

	TWeakObjectPtr<ACharacter> CachedOwnerCharacter; // 스킬 소유 캐릭터 캐시
	
};
