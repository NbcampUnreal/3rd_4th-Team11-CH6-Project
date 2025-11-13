#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "RInteractable.h"
#include "RInteractableActorBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FROnInteractableFocus, AActor*, Target, bool, bFocus);


UCLASS(Abstract)
class ARInteractableActorBase : public AActor, public IRInteractable
{
	GENERATED_BODY()
public:
	ARInteractableActorBase();


protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactable")
	USceneComponent* Root;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactable")
	UBoxComponent* Trigger;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable")
	float InteractRange = 180.f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable")
	bool bOneShot = true;


	UPROPERTY(BlueprintReadOnly, Category = "Interactable")
	bool bUsed = false;


public:
	UPROPERTY(BlueprintAssignable, Category = "Interactable")
	FROnInteractableFocus OnFocusChanged;


	virtual bool CanInteract_Implementation(AActor* ByActor) const override;
	virtual void Interact_Implementation(AActor* ByActor) override;
	virtual FText GetPrompt_Implementation() const override;


	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void SetFocused(AActor* ByActor, bool bFocus);


protected:
	UFUNCTION()
	void OnTriggerBegin(UPrimitiveComponent* OverComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Hit);


	UFUNCTION()
	void OnTriggerEnd(UPrimitiveComponent* OverComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex);
};
