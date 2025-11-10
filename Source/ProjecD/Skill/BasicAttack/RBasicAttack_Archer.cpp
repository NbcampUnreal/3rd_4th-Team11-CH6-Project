// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/BasicAttack/RBasicAttack_Archer.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

void URBasicAttack_Archer::ActivateSkill_Implementation()
{
	Super::ActivateSkill_Implementation();

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOuter());
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("URBasicAttack_Archer: OwnerCharacter is not valid."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Archer Attack: Firing %d arrows in a %.1f degree fan with %d penetration."), NumArrows, FanAngle, PenetrationCount);

	FVector StartLocation = OwnerCharacter->GetActorLocation();
	FRotator StartRotation = OwnerCharacter->GetActorRotation();
	float AttackRange = 3000.0f; // Long range for archer

	float HalfAngle = FanAngle / 2.0f;
	float AngleStep = (NumArrows > 1) ? FanAngle / (NumArrows - 1) : 0.0f;

	for (int32 i = 0; i < NumArrows; ++i)
	{
		float CurrentAngle = -HalfAngle + (i * AngleStep);
		FRotator ArrowRotation = StartRotation + FRotator(0, CurrentAngle, 0);
		FVector Direction = ArrowRotation.Vector();
		FVector EndLocation = StartLocation + Direction * AttackRange;

		TArray<FHitResult> HitResults;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(OwnerCharacter);

		UKismetSystemLibrary::LineTraceMulti(
			GetWorld(),
			StartLocation,
			EndLocation,
			UEngineTypes::ConvertToTraceType(ECC_Pawn),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			HitResults,
			true
		);

		// Sort hits by distance from the start
		HitResults.Sort([StartLocation](const FHitResult& A, const FHitResult& B) {
			return FVector::DistSquared(A.ImpactPoint, StartLocation) < FVector::DistSquared(B.ImpactPoint, StartLocation);
		});

		int32 HitCount = 0;
		TSet<AActor*> DamagedActorsThisArrow;
		for (const FHitResult& Hit : HitResults)
		{
			if (HitCount >= PenetrationCount)
			{
				break; // Stop after reaching penetration limit
			}

			AActor* HitActor = Hit.GetActor();
			if (HitActor && !DamagedActorsThisArrow.Contains(HitActor))
			{
				// Apply Damage here in the future
				UE_LOG(LogTemp, Log, TEXT("Archer Arrow %d Hit: %s with %.2f total damage."), i + 1, *HitActor->GetName(), GetTotalDamage());
				DamagedActorsThisArrow.Add(HitActor);
				HitCount++;
			}
		}
	}
}
