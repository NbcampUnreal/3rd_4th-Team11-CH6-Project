#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "R_AICharacter.generated.h"

UCLASS()
class PROJECD_API AR_AICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AR_AICharacter();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void PlayAttackMontage();

protected:
	UPROPERTY(EditAnywhere, Category="AI")
	UAnimMontage* AttackMontage;

};
