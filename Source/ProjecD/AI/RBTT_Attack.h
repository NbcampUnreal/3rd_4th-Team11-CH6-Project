#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RBTT_Attack.generated.h"


UCLASS()
class PROJECD_API URBTT_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	URBTT_Attack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
