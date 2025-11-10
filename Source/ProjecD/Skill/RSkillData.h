#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RSkillData.generated.h"

// ESkillType: 스킬의 종류를 정의합니다 (액티브, 패시브)
UENUM(BlueprintType)
enum class ESkillType : uint8
{
	EST_Active UMETA(DisplayName = "Active"),
	EST_Passive UMETA(DisplayName = "Passive")
};

// ECharacterClass: 스킬을 사용할 수 있는 직업을 정의합니다
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	ECC_Common UMETA(DisplayName = "Common"),
	ECC_Knight UMETA(DisplayName = "Knight"),
	ECC_Archer UMETA(DisplayName = "Archer"),
	ECC_Wizard UMETA(DisplayName = "Wizard")
};

// RSkillData: 데이터 테이블의 한 행을 구성할 스킬 데이터 구조체입니다.
USTRUCT(BlueprintType)
struct FRSkillData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 생성자: 멤버 변수들의 기본값을 설정합니다.
	FRSkillData() : SkillType(ESkillType::EST_Active), CharacterClass(ECharacterClass::ECC_Common), Cooldown(0.0f), ManaCost(0.0f) {}

	// UPROPERTY 매크로를 통해 언리얼 에디터에 노출될 변수들을 선언합니다.
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Data")
	TSubclassOf<class URSkillBase> SkillClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Data")
	int32 SkillID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Data")
	FText SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Data")
	FText SkillDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Data")
	TSoftObjectPtr<UTexture2D> SkillIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Data")
	ESkillType SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Data")
	ECharacterClass CharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Data")
	float Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Data")
	float ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Data")
	float BaseDamage;
};
