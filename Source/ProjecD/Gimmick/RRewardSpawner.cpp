// Fill out your copyright notice in the Description page of Project Settings.


#include "RRewardSpawner.h"
#include "Engine/World.h"


ARRewardSpawner::ARRewardSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}


void ARRewardSpawner::BeginPlay()
{
	Super::BeginPlay();
	// 보스 매니저/보스 캐릭터에서 이 액터의 OnBossKilled를 호출하도록 연결
}


void ARRewardSpawner::OnBossKilled(AActor* Boss, FVector Location)
{
	UWorld* World = GetWorld();
	if (!World) return;


	const FTransform PortalXform(FRotator::ZeroRotator, Location + PortalOffset);
	const FTransform ChestXform(FRotator::ZeroRotator, Location + ChestOffset);


	if (PortalClass)
	{
		World->SpawnActor<AActor>(PortalClass, PortalXform);
	}
	if (ChestClass)
	{
		World->SpawnActor<AActor>(ChestClass, ChestXform);
	}
}