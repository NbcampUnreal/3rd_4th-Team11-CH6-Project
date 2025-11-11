#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RBTT_Chase.generated.h"

UCLASS()
class PROJECD_API URBTT_Chase : public UBTTaskNode
{
	GENERATED_BODY()

public:
	URBTT_Chase();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
