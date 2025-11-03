#include "Core/RGameInstance.h"
#include "UI/Manager/ROutGameUIManager.h"

URGameInstance::URGameInstance()
{
	//나중에 기본적인 것추가
}

void URGameInstance::Init()
{
	Super::Init();

	UIManager=NewObject<UROutGameUIManager>(this);

	if (UIManager)
	{
		UE_LOG(LogTemp,Warning,TEXT("UIManager is created!"));
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("UIManager is not created!"));
	}
}
