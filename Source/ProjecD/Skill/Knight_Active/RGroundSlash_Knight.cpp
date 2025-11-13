// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Knight_Active/RGroundSlash_Knight.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemComponent.h" // GameplayEffect 적용을 위해
#include "AbilitySystemBlueprintLibrary.h" // UAbilitySystemBlueprintLibrary::MakeEffectContext 등

void URGroundSlash_Knight::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CachedOwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!CachedOwnerCharacter.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("URGroundSlash_Knight: OwnerCharacter가 유효하지 않습니다."));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 애니메이션 몽타주 재생
	if (GroundSlashMontage)
	{
		CachedOwnerCharacter->PlayAnimMontage(GroundSlashMontage);
	}

	// VFX 생성
	if (GroundSlashVFX)
	{
		UGameplayStatics::SpawnEmitterAttached(GroundSlashVFX, CachedOwnerCharacter->GetRootComponent(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
	}

	// 사운드 재생
	if (GroundSlashSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GroundSlashSound, CachedOwnerCharacter->GetActorLocation());
	}

	TArray<AActor*> OverlappedActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(CachedOwnerCharacter.Get());

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 모든 오브젝트 타입
	// ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // 특정 오브젝트 타입만 원한다면 추가

	FVector OwnerLocation = CachedOwnerCharacter->GetActorLocation();
	FVector OwnerForwardVector = CachedOwnerCharacter->GetActorForwardVector();
	FRotator OwnerRotation = CachedOwnerCharacter->GetActorRotation();

	// 박스 오버랩의 중심 계산 (캐릭터 전방으로 Range의 절반만큼 이동)
	FVector BoxCenter = OwnerLocation + OwnerForwardVector * (Range / 2.0f);
	// 박스 익스텐트 계산 (길이, 폭, 높이)
	FVector BoxExtent = FVector(Range / 2.0f, Width / 2.0f, 100.0f); // 높이는 임의로 100cm 설정

	// 박스 트레이스 수행 (오버랩처럼 사용)
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::BoxTraceMulti(
		GetWorld(),
		OwnerLocation, // Start
		OwnerLocation + OwnerForwardVector * Range, // End (Range만큼 전방으로 트레이스)
		BoxExtent, // HalfSize
		OwnerRotation, // Orientation
		UEngineTypes::ConvertToTraceType(ECC_Pawn), // TraceChannel
		false, // bTraceComplex
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration, // DrawDebugType
		HitResults,
		true // bIgnoreSelf
	);

	for (const FHitResult& Hit : HitResults)
	{
		AActor* TargetActor = Hit.GetActor();
		if (!TargetActor) continue;

		// TODO: TargetActor가 적인지 확인하는 로직 추가
		UE_LOG(LogTemp, Log, TEXT("GroundSlash hit %s for %f damage."), *TargetActor->GetName(), Damage);

		// 피해 적용 (GameplayEffect 사용 예시)
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (TargetASC)
		{
			// TODO: DamageGameplayEffectClass를 UPROPERTY로 추가하고 여기서 사용
			// 현재는 임시로 Damage 값을 사용
			FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
			EffectContext.AddInstigator(CachedOwnerCharacter.Get(), CachedOwnerCharacter.Get());
			EffectContext.SetAbility(this);

			// FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageGameplayEffectClass, 1, EffectContext);
			// SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), Damage);
			// TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}

	// 어빌리티 종료
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

