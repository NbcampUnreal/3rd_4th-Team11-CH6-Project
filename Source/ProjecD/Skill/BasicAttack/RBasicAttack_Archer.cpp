


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
		UE_LOG(LogTemp, Warning, TEXT("URBasicAttack_Archer: OwnerCharacter가 유효하지 않습니다."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("궁수 공격: %d개의 화살을 %.1f도 부채꼴로 %d 관통으로 발사합니다."), NumArrows, FanAngle, PenetrationCount);

	FVector StartLocation = OwnerCharacter->GetActorLocation();
	FRotator StartRotation = OwnerCharacter->GetActorRotation();
	float AttackRange = 3000.0f; // 궁수를 위한 긴 사거리

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

		// 시작 지점으로부터의 거리에 따라 히트 정렬
		HitResults.Sort([StartLocation](const FHitResult& A, const FHitResult& B) {
			return FVector::DistSquared(A.ImpactPoint, StartLocation) < FVector::DistSquared(B.ImpactPoint, StartLocation);
		});

		int32 HitCount = 0;
		TSet<AActor*> DamagedActorsThisArrow;
		for (const FHitResult& Hit : HitResults)
		{
			if (HitCount >= PenetrationCount)
			{
				break; // 관통 제한에 도달하면 중지
			}

			AActor* HitActor = Hit.GetActor();
			if (HitActor && !DamagedActorsThisArrow.Contains(HitActor))
			{
				// 향후 여기에 대미지 적용
				UE_LOG(LogTemp, Log, TEXT("궁수 화살 %d 적중: %s에게 총 %.2f 대미지."), i + 1, *HitActor->GetName(), GetTotalDamage());
				DamagedActorsThisArrow.Add(HitActor);
				HitCount++;
			}
		}
	}
}
