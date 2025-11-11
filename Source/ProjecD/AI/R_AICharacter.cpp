#include "AI/R_AICharacter.h"

// Sets default values
AR_AICharacter::AR_AICharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AR_AICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AR_AICharacter::PlayAttackMontage()
{
	if (AttackMontage && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
	}
}
