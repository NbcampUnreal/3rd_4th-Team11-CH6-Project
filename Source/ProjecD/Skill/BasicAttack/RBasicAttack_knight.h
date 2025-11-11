#pragma once

#include "CoreMinimal.h"
#include "Skill/RSkillBase.h"
#include "RBasicAttack_knight.generated.h"

/**
 * 기사(Knight)의 기본 공격 스킬 클래스입니다.
 * 3단 콤보 공격을 구현합니다.
 */
	UCLASS()
	class PROJECD_API URBasicAttack_knight : public URSkillBase
{
	GENERATED_BODY()

public:
	URBasicAttack_knight();

	virtual void ActivateSkill_Implementation() override;

protected:
	void ResetCombo();

	int32 ComboCount;

	FTimerHandle ComboResetTimer;

};