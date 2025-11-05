// RTitleGameMode.cpp

#include "Core/Title/RTitleGameMode.h"

void ARTitleGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("[RTitleGameMode] 타이틀 맵 시작"));
}