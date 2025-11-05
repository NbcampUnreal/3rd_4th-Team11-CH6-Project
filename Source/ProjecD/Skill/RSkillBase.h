#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h" // Required for FDataTableRowHandle
#include "RSkillData.h" // Include our new RSkillData.h
#include "RSkillBase.generated.h"

/**
 * 모든 스킬의 기본이 되는 클래스입니다.
 */
UCLASS(Blueprintable, BlueprintType) // 블루프린트에서 사용 가능하도록 설정
class PROJECD_API URSkillBase : public UObject
{
	GENERATED_BODY()

public:
	// 생성자
	URSkillBase();

	// 데이터 테이블에서 스킬 데이터를 참조합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FDataTableRowHandle SkillDataHandle;

	// 캐시된 스킬 데이터 (선택 사항, SkillDataHandle에서 로드될 수 있음)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	FRSkillData CachedSkillData;

	// 스킬 데이터를 사용하여 스킬을 초기화하는 함수
	UFUNCTION(BlueprintCallable, Category = "Skill")
	virtual void InitializeSkill(const FRSkillData& InSkillData);

	// 스킬을 활성화하는 가상 함수 (액티브 스킬용)
	UFUNCTION(BlueprintNativeEvent, Category = "Skill")
	void ActivateSkill();
	virtual void ActivateSkill_Implementation();

	// 패시브 효과를 적용하는 가상 함수 (패시브 스킬용)
	UFUNCTION(BlueprintNativeEvent, Category = "Skill")
	void ApplyPassiveEffect();
	virtual void ApplyPassiveEffect_Implementation();

	// 패시브 효과를 제거하는 가상 함수 (패시브 스킬용)
	UFUNCTION(BlueprintNativeEvent, Category = "Skill")
	void RemovePassiveEffect();
	virtual void RemovePassiveEffect_Implementation();

	// 스킬 타입 Getter
	UFUNCTION(BlueprintPure, Category = "Skill")
	ESkillType GetSkillType() const { return CachedSkillData.SkillType; }

	// 캐릭터 클래스 Getter
	UFUNCTION(BlueprintPure, Category = "Skill")
	ECharacterClass GetCharacterClass() const { return CachedSkillData.CharacterClass; }

protected:
	// 스킬이 생성되거나 초기화될 때 호출됩니다.
	virtual void PostInitProperties() override;
};
