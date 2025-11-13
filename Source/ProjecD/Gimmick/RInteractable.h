#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RInteractable.generated.h"

UINTERFACE(BlueprintType)
class URInteractable : public UInterface
{
	GENERATED_BODY()
};

class IRInteractable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	bool CanInteract(AActor* ByActor) const;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void Interact(AActor* ByActor);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	FText GetPrompt() const;
};
