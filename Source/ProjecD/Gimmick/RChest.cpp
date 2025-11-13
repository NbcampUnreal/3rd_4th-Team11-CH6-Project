#include "Gimmick/RChest.h"
#include "Kismet/GameplayStatics.h"


ARChest::ARChest()
{
	PrimaryActorTick.bCanEverTick = false;
	bOneShot = true;
}


void ARChest::BeginPlay()
{
	Super::BeginPlay();
}


FText ARChest::GetPrompt_Implementation() const
{
	return FText::FromString(TEXT("F:open chest"));
}


void ARChest::Interact_Implementation(AActor* ByActor)
{
	if (!CanInteract(ByActor)) return;
	OnChestOpened.Broadcast(this);
	UE_LOG(LogTemp, Log, TEXT("[RChest] Opened by %s"), *GetNameSafe(ByActor));
	if (bConsumeOnOpen) { bUsed = true; }
}
