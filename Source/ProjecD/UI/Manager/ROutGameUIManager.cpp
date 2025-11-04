#include "UI/Manager/ROutGameUIManager.h"
#include "UI/Base/RBaseOutGameWidget.h"
#include "Blueprint/UserWidget.h"

UROutGameUIManager::UROutGameUIManager()
{
	// 맴버 변수 초기화
	CurrentWidget=nullptr;
	PlayerController=nullptr;
}

void UROutGameUIManager::SetPlayerController(APlayerController* PC)
{
	PlayerController=PC;
	if (PlayerController)
	{
		UE_LOG(LogTemp,Warning,TEXT("UIManager: PlayerController Set!"));
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("UIManager: PlayerController is not Set!"));
	}
}

void UROutGameUIManager::ShowUI(TSubclassOf<URBaseOutGameWidget> WidgetClass)
{
	// 1. WidgetClass 유효성 체크
	if (!WidgetClass)
	{
		UE_LOG(LogTemp,Error,TEXT("UIManager: WidgetClass 가 nullptr!"));
		return;
	}
	
	// 2. 위젯 가져오기 또는 생성
	URBaseOutGameWidget* NewWidget=GetOrCreateWidget(WidgetClass);
	if (!NewWidget)
	{
		UE_LOG(LogTemp,Error,TEXT("UIManager: Widget 생성 실패!"));
		return;
	}
	
	//3. 현재 위젯 있으면 숨기기
	if (CurrentWidget)
	{
		CurrentWidget->HideUI();
	}
	
	//4. 새 위젯을 현재 위젯으로 설정
	CurrentWidget=NewWidget;

	//5. 스택에 추가
	WidgetStack.Add(CurrentWidget);

	//6. 위젯 표시
	CurrentWidget->ShowUI();

	UE_LOG(LogTemp,Log,TEXT("UIManager: %s 표시완료"), *WidgetClass->GetName());
	
}

void UROutGameUIManager::HideCurrentUI()
{
	if (CurrentWidget)
	{
		CurrentWidget->HideUI();
		CurrentWidget=nullptr;
		UE_LOG(LogTemp,Log,TEXT("UIManager: 현재 UI 숨김!!"));
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("UIManager: 숨길 UI가 없음!!"));
	}
}

void UROutGameUIManager::GoBackToPreviousUI()
{
	//1. 스택이 비어있는지 체크
	if (WidgetStack.Num()==0)
	{
		UE_LOG(LogTemp,Warning,TEXT("UIManager:뒤로 갈 UI가 없습니다!"));
		return;
	}

	//2. 현재 위젯 숨기기
	if (CurrentWidget)
	{
		CurrentWidget->HideUI();
	}

	//3. 스택에서 현재 위젯 제거
	WidgetStack.Pop();

	//4. 이전 위젯이 있으면 표시
	if (WidgetStack.Num()>0)
	{
		CurrentWidget=WidgetStack.Last();
		CurrentWidget->ShowUI();
		UE_LOG(LogTemp,Log,TEXT("UIManager: 이전 UI로 복귀!!"));
	}
	else
	{
		CurrentWidget=nullptr;
		UE_LOG(LogTemp,Log,TEXT("UIManager: 모든 UI가 닫힘!!"));
	}
}

void UROutGameUIManager::ClearAllUI()
{
	//1. 현재 위젯 있으면 숨기기
	if (CurrentWidget)
	{
		CurrentWidget->HideUI();
	}

	//2. 스택 초기화
	WidgetStack.Empty();

	//3. 현재 위젯 초기화
	CurrentWidget=nullptr;

	UE_LOG(LogTemp,Log,TEXT("UIManager: 모든 UI 제거 완료"));
}

URBaseOutGameWidget* UROutGameUIManager::GetOrCreateWidget(TSubclassOf<URBaseOutGameWidget> WidgetClass)
{
	//1. 캐시에 있는지 확인
	if (URBaseOutGameWidget** FoundWidget=CachedWidgets.Find(WidgetClass))
	{
		UE_LOG(LogTemp,Log,TEXT("UIManager: 캐시에서 위젯 가져옴 -%s"),*WidgetClass->GetName());
		return *FoundWidget;
	}

	//2. PlayerController 유효성 체크
	if (!PlayerController)
	{
		UE_LOG(LogTemp,Error,TEXT("UIManager: PlayerController가 없어서 위젯 생성 불가!!!"));
		return nullptr;
	}

	//3. 새로운 위젯 생성
	URBaseOutGameWidget* NewWidget=CreateWidget<URBaseOutGameWidget>(PlayerController,WidgetClass);

	if (NewWidget)
	{
		//4. 캐시에 추가
		CachedWidgets.Add(WidgetClass,NewWidget);
		UE_LOG(LogTemp, Log, TEXT("UIManager: 새 위젯 생성 - %s"), *WidgetClass->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UIManager: 위젯 생성 실패 - %s"), *WidgetClass->GetName());
	}

	return NewWidget;
	
}
