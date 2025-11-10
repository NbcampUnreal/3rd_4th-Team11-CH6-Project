

#pragma once

#include "CoreMinimal.h"
#include "Skill/RSkillBase.h"
#include "RBasicAttack_Archer.generated.h"

/**
 * 궁수(Archer)의 기본 공격 스킬 클래스입니다.
 * 부채꼴 모양으로 여러 발의 화살을 발사하는 로직을 포함합니다.
 */
UCLASS()
class PROJECD_API URBasicAttack_Archer : public URSkillBase
{
	GENERATED_BODY()

public:
	virtual void ActivateSkill_Implementation() override;

protected:
	// 런타임에 패시브에 의해 수정될 수 있습니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcherAttack")
	int32 NumArrows = 5;

	// 런타임에 패시브에 의해 수정될 수 있습니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcherAttack")
	float FanAngle = 30.0f;

	// 화살 하나가 관통할 수 있는 대상의 수입니다. 1은 관통 없음.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcherAttack")
	int32 PenetrationCount = 1;
	
};
