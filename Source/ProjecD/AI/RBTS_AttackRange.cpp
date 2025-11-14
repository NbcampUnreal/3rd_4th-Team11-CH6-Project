#include "AI/RBTS_AttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void URBTS_AttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AI = OwnerComp.GetAIOwner()->GetPawn();
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

	if (AI && Target)
	{
		float Distance = FVector::Dist(AI->GetActorLocation(), Target->GetActorLocation());
		bool bInRange = Distance <= AttackRange;

		OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsInAttackRange", bInRange);
	}
}

