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

	// 1단계: 초기 타겟 찾기
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
		UE_LOG(LogTemp, Log, TEXT("마법사 공격: 초기 타겟을 찾지 못했습니다."));
		return; // 타겟 없음, 공격 중지.
	}

	// 2단계: 연쇄 타겟에게 바운스
	AActor* CurrentTarget = FirstTarget;
	for (int32 i = 0; i < MaxBounces; ++i)
	{
		TArray<FHitResult> BounceHitResults;
		bool bFoundBounceTarget = UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(),
			CurrentTarget->GetActorLocation(),
			CurrentTarget->GetActorLocation(), // 전체 트레이스이므로 시작과 끝이 같음
			BounceRange,
			UEngineTypes::ConvertToTraceType(ECC_Pawn),
			false,
			HitTargets, // 이미 맞은 타겟은 제외
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
			// 다음 타겟을 더 이상 찾을 수 없음
			break;
		}
	}

	// 3단계: 효과 적용
	HitTargets.Remove(OwnerCharacter); // 무시 목록에서 자신 제거
	UE_LOG(LogTemp, Log, TEXT("마법사 공격: 연쇄 공격이 %d명의 타겟을 공격했습니다."), HitTargets.Num());
	for (AActor* Target : HitTargets)
	{
		// 데미지 적용 로직
		UE_LOG(LogTemp, Log, TEXT("...연쇄 공격: %s에게 총 %.2f 데미지."), *Target->GetName(), GetTotalDamage());
	}
}