#include "AI/RBTT_Chase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

URBTT_Chase::URBTT_Chase()
{
	NodeName = "Chase Player";
}

EBTNodeResult::Type URBTT_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TEXT("TargetActor")));

	if (Controller && Target)
	{
		Controller->MoveToActor(Target, 100.0f);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
