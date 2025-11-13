// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Knight_Active/RWhirlwind_Knight.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemComponent.h" // ApplyGameplayEffectToTarget 사용을 위해

// GameplayEffect 전방 선언 (피해 적용에 필요한 경우)
// #include "GameplayEffectTypes.h" 
// #include "GameplayEffect.h"

void URWhirlwind_Knight::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CachedOwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!CachedOwnerCharacter.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("URWhirlwind_Knight: OwnerCharacter가 유효하지 않습니다."));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 애니메이션 몽타주 재생
	if (WhirlwindMontage)
	{
		CachedOwnerCharacter->PlayAnimMontage(WhirlwindMontage);
	}

	// VFX 생성
	if (WhirlwindVFX)
	{
		UGameplayStatics::SpawnEmitterAttached(WhirlwindVFX, CachedOwnerCharacter->GetRootComponent(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
	}

	// 사운드 재생
	if (WhirlwindSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), WhirlwindSound, CachedOwnerCharacter->GetActorLocation());
	}

	// 주기적인 피해 틱 설정
	GetWorld()->GetTimerManager().SetTimer(WhirlwindTickTimerHandle, this, &URWhirlwind_Knight::WhirlwindTick, TickInterval, true);

	// 지속 시간 타이머 설정 (어빌리티 종료용)
	GetWorld()->GetTimerManager().SetTimer(WhirlwindDurationTimerHandle, this, &URWhirlwind_Knight::EndWhirlwind, Duration, false);
}

void URWhirlwind_Knight::WhirlwindTick()
{
	ACharacter* OwnerCharacter = CachedOwnerCharacter.Get();
	if (!OwnerCharacter)
	{
		EndWhirlwind(); // 소유자 캐릭터가 유효하지 않음, 어빌리티 종료
		return;
	}

	TArray<AActor*> OverlappedActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);

	// 적을 찾기 위한 캡슐 오버랩 수행
	UKismetSystemLibrary::CapsuleOverlapActors(
		GetWorld(),
		OwnerCharacter->GetActorLocation() + FVector(0, 0, HalfHeight), // 캡슐의 중심 위치 조정
		Radius,
		HalfHeight,
		TArray<TEnumAsByte<EObjectTypeQuery>>(), // 모든 오브젝트 타입
		AActor::StaticClass(), // 모든 액터 타입
		ActorsToIgnore,
		OverlappedActors
	);

	for (AActor* TargetActor : OverlappedActors)
	{
		// TODO: TargetActor가 적인지 확인하는 로직 추가
		// 현재는 피해량만 로그로 출력
		UE_LOG(LogTemp, Log, TEXT("Whirlwind hit %s for %f damage."), *TargetActor->GetName(), Damage);

		// GameplayEffect를 통한 피해 적용 예시 (DamageGameplayEffect TSubclassOf 속성 필요)
		/*
		UAbilitySystemComponent* TargetASC = UGameplayStatics::GetAbilitySystemComponent(TargetActor);
		if (TargetASC && DamageGameplayEffectClass)
		{
			FGameplayEffectContextHandle EffectContext = TargetASC->Make  EffectContext();
			EffectContext.AddInstigator(this, OwnerCharacter);
			EffectContext.SetAbility(this);

			FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageGameplayEffectClass, 1, EffectContext);
			SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), Damage); // 피해 태그 가정
			TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
		*/
	}
}

void URWhirlwind_Knight::EndWhirlwind()
{
	GetWorld()->GetTimerManager().ClearTimer(WhirlwindTickTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(WhirlwindDurationTimerHandle);

	// 어빌리티 종료
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

