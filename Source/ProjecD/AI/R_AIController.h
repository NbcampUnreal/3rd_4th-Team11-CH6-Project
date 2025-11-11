#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "R_AIController.generated.h"


UCLASS()
class PROJECD_API AR_AIController : public AAIController
{
	GENERATED_BODY()

public:
	AR_AIController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	
	void SetPlayerTarget(APawn* PlayerPawn);

protected:
	UPROPERTY(EditDefaultsOnly, Category="AI")
	UBehaviorTree* BehaviorTreeAsset;

private:
	UBlackboardComponent* BlackboardComp;
	
};
