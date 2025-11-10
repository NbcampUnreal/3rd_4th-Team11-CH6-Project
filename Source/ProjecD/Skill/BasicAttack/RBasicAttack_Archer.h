// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/RSkillBase.h"
#include "RBasicAttack_Archer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECD_API URBasicAttack_Archer : public URSkillBase
{
	GENERATED_BODY()

public:
	virtual void ActivateSkill_Implementation() override;

protected:
	// Can be modified by passives at runtime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcherAttack")
	int32 NumArrows = 5;

	// Can be modified by passives at runtime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcherAttack")
	float FanAngle = 30.0f;

	// How many targets one arrow can penetrate. 1 = no penetration.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcherAttack")
	int32 PenetrationCount = 1;
	
};
