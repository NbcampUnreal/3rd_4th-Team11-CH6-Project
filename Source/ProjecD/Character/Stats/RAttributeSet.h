#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "RAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class PROJECD_API URAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	URAttributeSet();

	// Primary
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Str)
	FGameplayAttributeData Str;
	ATTRIBUTE_ACCESSORS(URAttributeSet, Str)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Dex)
	FGameplayAttributeData Dex;
	ATTRIBUTE_ACCESSORS(URAttributeSet, Dex)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Con)
	FGameplayAttributeData Con;
	ATTRIBUTE_ACCESSORS(URAttributeSet, Con)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Int)
	FGameplayAttributeData Int;
	ATTRIBUTE_ACCESSORS(URAttributeSet, Int)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Win)
	FGameplayAttributeData Win;
	ATTRIBUTE_ACCESSORS(URAttributeSet, Win)

		// Derived
		UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_AttackPower)
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(URAttributeSet, AttackPower)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Defense)
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(URAttributeSet, Defense)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(URAttributeSet, MoveSpeed)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_AttackSpeed)
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(URAttributeSet, AttackSpeed)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URAttributeSet, MaxHealth)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_HealthRegen)
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(URAttributeSet, HealthRegen)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(URAttributeSet, MaxMana)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MagicPower)
	FGameplayAttributeData MagicPower;
	ATTRIBUTE_ACCESSORS(URAttributeSet, MagicPower)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_ManaRegen)
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(URAttributeSet, ManaRegen)

		// Current resources
		UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(URAttributeSet, CurrentHealth)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_CurrentMana)
	FGameplayAttributeData CurrentMana;
	ATTRIBUTE_ACCESSORS(URAttributeSet, CurrentMana)

		// Overrides
		virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	void RecalculateDerivedAttributes(const FGameplayEffectModCallbackData* Data = nullptr);

	// OnRep callbacks
	UFUNCTION()
	void OnRep_Str(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(URAttributeSet, Str, Old); }
	UFUNCTION()
	void OnRep_Dex(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(URAttributeSet, Dex, Old); }
	UFUNCTION()
	void OnRep_Con(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(URAttributeSet, Con, Old); }
	UFUNCTION()
	void OnRep_Int(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(URAttributeSet, Int, Old); }
	UFUNCTION()
	void OnRep_Win(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(URAttributeSet, Win, Old); }

	UFUNCTION()
	void OnRep_AttackPower(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(URAttributeSet, AttackPower, Old); }
	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(URAttributeSet, Defense, Old); }
	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(URAttributeSet, MoveSpeed, Old); }
	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(URAttributeSet, AttackSpeed, Old); }
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(URAttributeSet, MaxHealth, Old); }
	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(URAttributeSet, HealthRegen, Old); }
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(URAttributeSet, MaxMana, Old); }
	UFUNCTION()
	void OnRep_MagicPower(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(URAttributeSet, MagicPower, Old); }
	UFUNCTION()
	void OnRep_ManaRegen(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(URAttributeSet, ManaRegen, Old); }

	UFUNCTION()
	void OnRep_CurrentHealth(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(URAttributeSet, CurrentHealth, Old); }
	UFUNCTION()
	void OnRep_CurrentMana(const FGameplayAttributeData& Old) { GAMEPLAYATTRIBUTE_REPNOTIFY(URAttributeSet, CurrentMana, Old); }
};
