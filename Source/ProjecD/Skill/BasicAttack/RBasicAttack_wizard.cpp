


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
		UE_LOG(LogTemp, Warning, TEXT("URBasicAttack_wizard: OwnerCharacter가 유효하지 않습니다."));
		return;
	}

	TArray<AActor*> HitTargets;
	HitTargets.Add(OwnerCharacter); // 무시 목록에 자신 추가

	// 1단계: 초기 대상 찾기
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
		UE_LOG(LogTemp, Log, TEXT("마법사 공격: 초기 대상을 찾을 수 없습니다."));
		return; // 대상 없음, 연쇄 없음.
	}

	// 2단계: 후속 대상에게 연쇄
	AActor* CurrentTarget = FirstTarget;
	for (int32 i = 0; i < MaxBounces; ++i)
	{
		TArray<FHitResult> BounceHitResults;
		bool bFoundBounceTarget = UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(),
			CurrentTarget->GetActorLocation(),
			CurrentTarget->GetActorLocation(), // 구체 트레이스의 시작과 끝은 동일
			BounceRange,
			UEngineTypes::ConvertToTraceType(ECC_Pawn),
			false,
			HitTargets, // 이미 맞은 액터는 무시
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
			// 범위 내에서 더 이상 대상을 찾을 수 없음
			break;
		}
	}

	// 3단계: 효과 적용
	HitTargets.Remove(OwnerCharacter); // 최종 목록에서 자신 제거
	UE_LOG(LogTemp, Log, TEXT("마법사 공격: 연쇄 번개가 %d개의 대상을 적중했습니다."), HitTargets.Num());
	for (AActor* Target : HitTargets)
	{
		// 향후 여기에 대미지 적용
		UE_LOG(LogTemp, Log, TEXT("...적중 대상: %s에게 총 %.2f 대미지."), *Target->GetName(), GetTotalDamage());
	}
}