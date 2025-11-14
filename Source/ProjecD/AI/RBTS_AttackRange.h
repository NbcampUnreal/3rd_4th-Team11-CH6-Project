#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RBTS_AttackRange.generated.h"

UCLASS()
class PROJECD_API URBTS_AttackRange : public UBTService
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="AI")
	float AttackRange = 200.f;

	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime);
};
