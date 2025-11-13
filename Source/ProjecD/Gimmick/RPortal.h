#pragma once
#include "CoreMinimal.h"
#include "Gimmick/RInteractableActorBase.h"
#include "RPortal.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FROnPortalUsed, AActor*, ByActor);


UCLASS()
class ARPortal : public ARInteractableActorBase
{
	GENERATED_BODY()
public:
	ARPortal();


protected:
	virtual void BeginPlay() override;


public:
	// 이동할 맵 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Portal")
	FName TargetMapName = TEXT("Lobby");


	// 이동 전 SaveGame 등 처리용
	UFUNCTION(BlueprintImplementableEvent, Category = "Portal")
	void OnBeforeTravel(AActor* ByActor);


	UPROPERTY(BlueprintAssignable, Category = "Portal|Events")
	FROnPortalUsed OnPortalUsed;


public:
	virtual void Interact_Implementation(AActor* ByActor) override;
	virtual FText GetPrompt_Implementation() const override;
};