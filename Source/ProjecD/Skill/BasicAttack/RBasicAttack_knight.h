// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/RSkillBase.h"
#include "RBasicAttack_knight.generated.h"

/**
 * 
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
