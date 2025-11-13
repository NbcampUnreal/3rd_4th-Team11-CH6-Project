#include "Gimmick/RPortal.h"
#include "Kismet/GameplayStatics.h"


ARPortal::ARPortal()
{
	PrimaryActorTick.bCanEverTick = false;
	bOneShot = false; // 여러 번 사용 가능
}


void ARPortal::BeginPlay()
{
	Super::BeginPlay();
}


FText ARPortal::GetPrompt_Implementation() const
{
	return FText::FromString(TEXT("F:return to lobby"));
}


void ARPortal::Interact_Implementation(AActor* ByActor)
{
	if (!CanInteract(ByActor)) return;
	OnPortalUsed.Broadcast(ByActor);
	OnBeforeTravel(ByActor);


	if (TargetMapName != NAME_None)
	{
		UGameplayStatics::OpenLevel(this, TargetMapName);
	}
}

