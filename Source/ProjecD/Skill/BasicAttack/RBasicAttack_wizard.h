// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/RSkillBase.h"
#include "RBasicAttack_wizard.generated.h"

/**
 * 
 */
UCLASS()
class PROJECD_API URBasicAttack_wizard : public URSkillBase
{
	GENERATED_BODY()

public:
	virtual void ActivateSkill_Implementation() override;

protected:
	// Can be modified by passives at runtime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WizardAttack")
	int32 MaxBounces = 9;

	// Can be modified by passives at runtime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WizardAttack")
	float InitialRange = 2000.0f;

	// Can be modified by passives at runtime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WizardAttack")
	float BounceRange = 1000.0f;
	
};
