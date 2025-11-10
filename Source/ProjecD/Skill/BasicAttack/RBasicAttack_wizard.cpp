// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/BasicAttack/RBasicAttack_wizard.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

void URBasicAttack_wizard::ActivateSkill_Implementation()
{
	Super::ActivateSkill_Implementation();

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOuter());
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("URBasicAttack_wizard: OwnerCharacter is not valid."));
		return;
	}

	TArray<AActor*> HitTargets;
	HitTargets.Add(OwnerCharacter); // Add self to ignore list

	// Step 1: Find the initial target
	AActor* FirstTarget = nullptr;
	FVector StartLocation = OwnerCharacter->GetActorLocation();
	FVector EndLocation = StartLocation + OwnerCharacter->GetActorForwardVector() * InitialRange;
	FHitResult HitResult;

	bool bHit = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		StartLocation,
		EndLocation,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		HitTargets,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	if (bHit)
	{
		FirstTarget = HitResult.GetActor();
		HitTargets.Add(FirstTarget);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Wizard Attack: No initial target found."));
		return; // No target, no chain.
	}

	// Step 2: Chain to subsequent targets
	AActor* CurrentTarget = FirstTarget;
	for (int32 i = 0; i < MaxBounces; ++i)
	{
		TArray<FHitResult> BounceHitResults;
		bool bFoundBounceTarget = UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(),
			CurrentTarget->GetActorLocation(),
			CurrentTarget->GetActorLocation(), // Start and end are the same for a sphere trace at a location
			BounceRange,
			UEngineTypes::ConvertToTraceType(ECC_Pawn),
			false,
			HitTargets, // Ignore actors that have already been hit
			EDrawDebugTrace::ForDuration,
			BounceHitResults,
			true
		);

		AActor* NextTarget = nullptr;
		float MinDistanceSq = FMath::Square(BounceRange);

		if (bFoundBounceTarget)
		{
			for (const FHitResult& BounceHit : BounceHitResults)
			{
				float DistanceSq = FVector::DistSquared(CurrentTarget->GetActorLocation(), BounceHit.GetActor()->GetActorLocation());
				if (DistanceSq < MinDistanceSq)
				{
					MinDistanceSq = DistanceSq;
					NextTarget = BounceHit.GetActor();
				}
			}
		}

		if (NextTarget)
		{
			HitTargets.Add(NextTarget);
			CurrentTarget = NextTarget;
		}
		else
		{
			// No more targets found in range
			break;
		}
	}

	// Step 3: Apply effects
	HitTargets.Remove(OwnerCharacter); // Remove self from the final list
	UE_LOG(LogTemp, Log, TEXT("Wizard Attack: Chain lightning hit %d targets."), HitTargets.Num());
	for (AActor* Target : HitTargets)
	{
		// Apply Damage here in the future
		UE_LOG(LogTemp, Log, TEXT("...Hit Target: %s with %.2f total damage."), *Target->GetName(), GetTotalDamage());
	}
}