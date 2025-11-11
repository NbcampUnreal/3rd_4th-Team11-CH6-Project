#include "Skill/BasicAttack/RBasicAttack_knight.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"

URBasicAttack_knight::URBasicAttack_knight()
{
	ComboCount = 0;
}

void URBasicAttack_knight::ActivateSkill_Implementation()
{
	Super::ActivateSkill_Implementation();

	// 콤보 리셋 타이머를 초기화합니다.
	GetWorld()->GetTimerManager().ClearTimer(ComboResetTimer);

	ComboCount++;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOuter());
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("URBasicAttack_knight: OwnerCharacter가 유효하지 않습니다."));
		return;
	}

	FVector Start = OwnerCharacter->GetActorLocation();
	FVector ForwardVector = OwnerCharacter->GetActorForwardVector();
	float AttackRange = 200.0f; // 공격 범위
	float AttackRadius = 50.0f;

	FVector End;
	FRotator Rotation;

	switch (ComboCount)
	{
	case 1:
		// 1번 콤보: 휘두르기
		UE_LOG(LogTemp, Log, TEXT("기사 공격: 1번 콤보 (휘두르기)"));
		End = Start + ForwardVector * AttackRange;
		Rotation = OwnerCharacter->GetActorRotation();
		break;
	case 2:
		// 2번 콤보: 대각선 베기
		UE_LOG(LogTemp, Log, TEXT("기사 공격: 2번 콤보 (대각선 베기)"));
		Rotation = OwnerCharacter->GetActorRotation() + FRotator(0, -45.0f, 0);
		End = Start + Rotation.Vector() * AttackRange;
		break;
	case 3:
		// 3번 콤보: 찌르기
		UE_LOG(LogTemp, Log, TEXT("기사 공격: 3번 콤보 (찌르기)"));
		AttackRadius = 75.0f; // 마지막 공격은 더 넓은 반경
		End = Start + ForwardVector * AttackRange;
		Rotation = OwnerCharacter->GetActorRotation();
		ComboCount = 0; // 3번 공격 후 콤보 초기화
		break;
	default:
		ComboCount = 0;
		return; // 발생해서는 안 됨
	}

	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);

	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		Start,
		End,
		AttackRadius,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			// 맞은 액터에게 데미지 적용
			UE_LOG(LogTemp, Log, TEXT("기사 공격 성공: %s에게 %.2f 데미지."), *Hit.GetActor()->GetName(), GetTotalDamage());
		}
	}

	// 새로운 공격이 없으면 콤보를 초기화하는 타이머 설정
	if (ComboCount != 0)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboResetTimer, this, &URBasicAttack_knight::ResetCombo, 1.5f, false);
	}
}

void URBasicAttack_knight::ResetCombo()
{
	UE_LOG(LogTemp, Log, TEXT("기사 공격: 콤보 초기화"));
	ComboCount = 0;
}
