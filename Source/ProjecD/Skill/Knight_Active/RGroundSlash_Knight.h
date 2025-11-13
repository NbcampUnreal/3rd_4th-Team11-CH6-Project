// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/RSkillBase.h"
#include "Animation/AnimMontage.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "RGroundSlash_Knight.generated.h"

/**
 * 
 */
UCLASS()
class PROJECD_API URGroundSlash_Knight : public URSkillBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GroundSlash")
	float Damage = 30.0f; // 스킬 피해량

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GroundSlash")
	float Range = 800.0f; // 지면 베기 길이

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GroundSlash")
	float Width = 200.0f; // 지면 베기 폭

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GroundSlash")
	UAnimMontage* GroundSlashMontage; // 재생할 애니메이션 몽타주

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GroundSlash")
	UParticleSystem* GroundSlashVFX; // 재생할 시각 효과

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GroundSlash")
	USoundCue* GroundSlashSound; // 재생할 사운드

	TWeakObjectPtr<ACharacter> CachedOwnerCharacter; // 스킬 소유 캐릭터 캐시
	
};
