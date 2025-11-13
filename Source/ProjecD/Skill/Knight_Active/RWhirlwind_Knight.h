// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/RSkillBase.h"
#include "Animation/AnimMontage.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "RWhirlwind_Knight.generated.h"

/**
 * 기사의 휠윈드 액티브 스킬 클래스입니다.
 * 원통형 범위 내의 적에게 주기적으로 피해를 줍니다.
 */
UCLASS()
class PROJECD_API URWhirlwind_Knight : public URSkillBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Whirlwind")
	float Damage = 10.0f; // 스킬 피해량

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Whirlwind")
	float Radius = 300.0f; // 휠윈드 반경

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Whirlwind")
	float HalfHeight = 100.0f; // 캡슐의 절반 높이 (원통형 모양을 위함)

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Whirlwind")
	float Duration = 3.0f; // 휠윈드 지속 시간

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Whirlwind")
	float TickInterval = 0.5f; // 피해 적용 간격

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Whirlwind")
	UAnimMontage* WhirlwindMontage; // 재생할 애니메이션 몽타주

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Whirlwind")
	UParticleSystem* WhirlwindVFX; // 재생할 시각 효과

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Whirlwind")
	USoundCue* WhirlwindSound; // 재생할 사운드

	TWeakObjectPtr<ACharacter> CachedOwnerCharacter; // 스킬 소유 캐릭터 캐시

	FTimerHandle WhirlwindTickTimerHandle; // 휠윈드 피해 틱 타이머 핸들
	FTimerHandle WhirlwindDurationTimerHandle; // 휠윈드 지속 시간 타이머 핸들

	UFUNCTION()
	void WhirlwindTick(); // 주기적인 피해 적용 함수

	UFUNCTION()
	void EndWhirlwind(); // 휠윈드 스킬 종료 함수
	
};
