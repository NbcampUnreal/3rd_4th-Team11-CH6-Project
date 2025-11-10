// Fill out your copyright notice in the Description page of Project Settings.


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

	// Clear any existing timer to ensure combo can continue
	GetWorld()->GetTimerManager().ClearTimer(ComboResetTimer);

	ComboCount++;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOuter());
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("URBasicAttack_knight: OwnerCharacter is not valid."));
		return;
	}

	FVector Start = OwnerCharacter->GetActorLocation();
	FVector ForwardVector = OwnerCharacter->GetActorForwardVector();
	float AttackRange = 200.0f; // Increased range
	float AttackRadius = 50.0f;

	FVector End;
	FRotator Rotation;

	switch (ComboCount)
	{
	case 1:
		// 1st Combo: Horizontal slash
		UE_LOG(LogTemp, Log, TEXT("Knight Attack: Combo 1 (Horizontal)"));
		End = Start + ForwardVector * AttackRange;
		Rotation = OwnerCharacter->GetActorRotation();
		break;
	case 2:
		// 2nd Combo: Diagonal slash (to the left)
		UE_LOG(LogTemp, Log, TEXT("Knight Attack: Combo 2 (Diagonal)"));
		Rotation = OwnerCharacter->GetActorRotation() + FRotator(0, -45.0f, 0);
		End = Start + Rotation.Vector() * AttackRange;
		break;
	case 3:
		// 3rd Combo: Vertical slash (strong forward attack)
		UE_LOG(LogTemp, Log, TEXT("Knight Attack: Combo 3 (Vertical)"));
		AttackRadius = 75.0f; // Larger radius for final attack
		End = Start + ForwardVector * AttackRange;
		Rotation = OwnerCharacter->GetActorRotation();
		ComboCount = 0; // Reset combo after 3rd attack
		break;
	default:
		ComboCount = 0;
		return; // Should not happen
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
			// Apply Damage here in the future
			UE_LOG(LogTemp, Log, TEXT("Knight Attack Hit: %s with %.2f total damage."), *Hit.GetActor()->GetName(), GetTotalDamage());
		}
	}

	// Set a timer to reset the combo if no new attack is made
	if (ComboCount != 0)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboResetTimer, this, &URBasicAttack_knight::ResetCombo, 1.5f, false);
	}
}

void URBasicAttack_knight::ResetCombo()
{
	UE_LOG(LogTemp, Log, TEXT("Knight Attack: Combo Reset"));
	ComboCount = 0;
}