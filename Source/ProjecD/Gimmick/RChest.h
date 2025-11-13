#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gimmick/RInteractableActorBase.h"
#include "RChest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FROnChestOpened, class ARChest*, Chest);


UCLASS()
class ARChest : public ARInteractableActorBase
{
	GENERATED_BODY()
public:
	ARChest();


protected:
	virtual void BeginPlay() override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chest")
	bool bConsumeOnOpen = true;


	UPROPERTY(BlueprintAssignable, Category = "Chest|Events")
	FROnChestOpened OnChestOpened;


public:
	virtual void Interact_Implementation(AActor* ByActor) override;
	virtual FText GetPrompt_Implementation() const override;
};