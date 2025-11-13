#include "Gimmick/RInteractableActorBase.h"
#include "Components/BoxComponent.h"

ARInteractableActorBase::ARInteractableActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Root);
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Trigger->SetBoxExtent(FVector(120.f));
}


void ARInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ARInteractableActorBase::OnTriggerBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ARInteractableActorBase::OnTriggerEnd);
}


bool ARInteractableActorBase::CanInteract_Implementation(AActor* ByActor) const
{
	if (bUsed && bOneShot) return false;
	if (!ByActor) return false;
	const float DistSq = FVector::DistSquared(ByActor->GetActorLocation(), GetActorLocation());
	return DistSq <= FMath::Square(InteractRange);
}


void ARInteractableActorBase::Interact_Implementation(AActor* ByActor)
{
	if (bOneShot) { bUsed = true; }
}


FText ARInteractableActorBase::GetPrompt_Implementation() const
{
	return FText::FromString(TEXT("F:Interactable"));
}


void ARInteractableActorBase::SetFocused(AActor* ByActor, bool bFocus)
{
	OnFocusChanged.Broadcast(this, bFocus);
}


void ARInteractableActorBase::OnTriggerBegin(UPrimitiveComponent* OverComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (Other && Other != this)
	{
		OnFocusChanged.Broadcast(this, true);
	}
}


void ARInteractableActorBase::OnTriggerEnd(UPrimitiveComponent* OverComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex)
{
	if (Other && Other != this)
	{
		OnFocusChanged.Broadcast(this, false);
	}
}