#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RRewardSpawner.generated.h"

UCLASS()
class ARRewardSpawner : public AActor
{
	GENERATED_BODY()
public:
	ARRewardSpawner();


protected:
	virtual void BeginPlay() override;


public:
	
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AActor> PortalClass; 


	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AActor> ChestClass; 


	// 보스 처치 시 호출
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void OnBossKilled(AActor* Boss, FVector Location);


	// 보스 위치 기준 오프셋들
	UPROPERTY(EditAnywhere, Category = "Spawn")
	FVector PortalOffset = FVector(0, 0, 0);


	UPROPERTY(EditAnywhere, Category = "Spawn")
	FVector ChestOffset = FVector(200, 0, 0);
};