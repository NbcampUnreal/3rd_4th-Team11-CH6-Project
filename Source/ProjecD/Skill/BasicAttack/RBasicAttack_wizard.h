#pragma once

#include "CoreMinimal.h"
#include "Skill/RSkillBase.h"
#include "RBasicAttack_wizard.generated.h"

/**
 * 마법사(Wizard)의 기본 공격 스킬 클래스입니다.
 * 연쇄 번개 공격을 구현합니다.
 */
	UCLASS()
	class PROJECD_API URBasicAttack_wizard : public URSkillBase
{
	GENERATED_BODY()

public:
	virtual void ActivateSkill_Implementation() override;

protected:
	// 런타임에 패시브에 의해 변경될 수 있습니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WizardAttack")
	int32 MaxBounces = 9;

	// 런타임에 패시브에 의해 변경될 수 있습니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WizardAttack")
	float InitialRange = 2000.0f;

	// 런타임에 패시브에 의해 변경될 수 있습니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WizardAttack")
	float BounceRange = 1000.0f;

};