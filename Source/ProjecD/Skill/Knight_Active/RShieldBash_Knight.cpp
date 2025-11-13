// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Knight_Active/RShieldBash_Knight.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemComponent.h" // GameplayEffect 적용을 위해
#include "AbilitySystemBlueprintLibrary.h" // UAbilitySystemBlueprintLibrary::MakeEffectContext 등

void URShieldBash_Knight::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CachedOwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!CachedOwnerCharacter.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("URShieldBash_Knight: OwnerCharacter가 유효하지 않습니다."));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 애니메이션 몽타주 재생
	if (ShieldBashMontage)
	{
		CachedOwnerCharacter->PlayAnimMontage(ShieldBashMontage);
	}

	// VFX 생성
	if (ShieldBashVFX)
	{
		UGameplayStatics::SpawnEmitterAttached(ShieldBashVFX, CachedOwnerCharacter->GetRootComponent(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
	}

	// 사운드 재생
	if (ShieldBashSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShieldBashSound, CachedOwnerCharacter->GetActorLocation());
	}

	TArray<AActor*> OverlappedActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(CachedOwnerCharacter.Get());

	// 부채꼴 모양의 오버랩을 위해 SphereOverlapActors를 사용하고 각도로 필터링
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		CachedOwnerCharacter->GetActorLocation(),
		Range, // SphereOverlap의 반경으로 Range 사용
		TArray<TEnumAsByte<EObjectTypeQuery>>(), // 모든 오브젝트 타입
		AActor::StaticClass(), // 모든 액터 타입
		ActorsToIgnore,
		OverlappedActors
	);

	FVector OwnerForwardVector = CachedOwnerCharacter->GetActorForwardVector();
	FVector OwnerLocation = CachedOwnerCharacter->GetActorLocation();

	for (AActor* TargetActor : OverlappedActors)
	{
		// TODO: TargetActor가 적인지 확인하는 로직 추가
		// 타겟까지의 벡터 계산
		FVector ToTarget = (TargetActor->GetActorLocation() - OwnerLocation);
		ToTarget.Normalize();

		// 전방 벡터와 타겟 벡터 사이의 각도 계산
		float DotProduct = FVector::DotProduct(OwnerForwardVector, ToTarget);
		float AngleRad = FMath::Acos(DotProduct);
		float AngleDeg = FMath::RadiansToDegrees(AngleRad);

		// 타겟이 부채꼴 범위 내에 있는지 확인
		if (AngleDeg <= (Angle / 2.0f))
		{
			UE_LOG(LogTemp, Log, TEXT("ShieldBash hit %s for %f damage and stun."), *TargetActor->GetName(), Damage);

			// 피해 적용 (GameplayEffect 사용 예시)
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
			if (TargetASC)
			{
				// 피해 GameplayEffect 적용
				// TODO: DamageGameplayEffectClass를 UPROPERTY로 추가하고 여기서 사용
				// 현재는 임시로 Damage 값을 사용
				FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
				EffectContext.AddInstigator(CachedOwnerCharacter.Get(), CachedOwnerCharacter.Get());
				EffectContext.SetAbility(this);

				// 기절 GameplayEffect 적용
				if (StunGameplayEffectClass)
				{
					FGameplayEffectSpecHandle StunSpecHandle = TargetASC->MakeOutgoingSpec(StunGameplayEffectClass, 1, EffectContext);
					// 기절 지속 시간 설정 (GameplayEffect 내에서 Duration Policy를 Infinite로 하고 SetByCaller로 Duration을 설정하는 경우)
					// StunSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.StunDuration")), StunDuration);
					TargetASC->ApplyGameplayEffectSpecToSelf(*StunSpecHandle.Data.Get());
				}
			}
		}
	}

	// 어빌리티 종료
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

