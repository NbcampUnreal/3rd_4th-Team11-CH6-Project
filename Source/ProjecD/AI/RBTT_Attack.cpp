#include "AI/RBTT_Attack.h"
#include "AIController.h"
#include "R_AICharacter.h"

URBTT_Attack::URBTT_Attack()
{
	NodeName = "Attack Player";
}

EBTNodeResult::Type URBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	
	AR_AICharacter* AIChar = Controller ? Cast<AR_AICharacter>(Controller->GetPawn()) : nullptr;

	if (AIChar)
	{
		AIChar->PlayAttackMontage();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}