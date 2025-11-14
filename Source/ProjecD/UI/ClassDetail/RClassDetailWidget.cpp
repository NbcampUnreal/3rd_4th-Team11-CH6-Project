#include "UI/ClassDetail/RClassDetailWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Core/RGameInstance.h"
#include "Kismet/GameplayStatics.h"

void URClassDetailWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//왼쪽 카드 버튼 바인딩
	if (ClassInfoButton)
	{
		ClassInfoButton->OnClicked.Clear();
		ClassInfoButton->OnClicked.AddDynamic(this,&URClassDetailWidget::OnClassInfoButtonClicked);
	}

	if (BasicAttackButton)
	{
		BasicAttackButton->OnClicked.Clear();
		BasicAttackButton->OnClicked.AddDynamic(this,&URClassDetailWidget::OnBasicAttackButtonClicked);
	}
	
	//하단 버튼 바인딩
	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.Clear();
		ConfirmButton->OnClicked.AddDynamic(this,&URClassDetailWidget::OnConfirmClicked);
	}
	if (CancelButton)
	{
		CancelButton->OnClicked.Clear();
		CancelButton->OnClicked.AddDynamic(this,&URClassDetailWidget::OnCancelClicked);
	}

	UE_LOG(LogTemp,Log,TEXT("RClassDetailWidget 초기화 완료"));
}

void URClassDetailWidget::SetClassInfo(ECharacterClassType ClassType)
{
	CurrentClassType=ClassType;

	//직업 고유 정보를 표시
	OnClassInfoButtonClicked();

	UE_LOG(LogTemp,Log,TEXT("클래스 정보 설정 완료: %d"),(int32)ClassType);
}

void URClassDetailWidget::OnClassInfoButtonClicked()
{
	FString Title,Content;
	GetClassDescription(Title,Content);

	if (DetailTitleText)
	{
		DetailTitleText->SetText(FText::FromString(Title));
	}

	if (DetailContentText)
	{
		DetailContentText->SetText(FText::FromString(Content));
	}

	UE_LOG(LogTemp,Log,TEXT("직업 소개 표시"));
}

void URClassDetailWidget::OnBasicAttackButtonClicked()
{
	FString Title,Content;
	GetBasicAttackDescription(Title,Content);

	if (DetailTitleText)
	{
		DetailTitleText->SetText(FText::FromString(Title));
	}

	if (DetailContentText)
	{
		DetailContentText->SetText(FText::FromString(Content));
		
	}

	UE_LOG(LogTemp,Log,TEXT("기본 공격 설명 표시"));
}

void URClassDetailWidget::GetClassDescription(FString& OutTitle, FString& OutContent) const
{
	switch (CurrentClassType)
	{
	case ECharacterClassType::Knight:
		OutTitle=TEXT("기사");
		OutContent=TEXT("역할군: 근접 딜러\n");
		OutContent+=TEXT("강력한 근접 계열 클래스이며, 강력한 콤보 공격을 구사합니다.\n\n");
		OutContent+=TEXT("기사는 검술의 달인이자 근접 전투의 전문가 입니다.\n");
		OutContent+=TEXT("그가 휘두르는 검을 피해 살아나간 적은 없습니다.\n");
		break;

	case ECharacterClassType::Archer:
		OutTitle=TEXT("궁수");
		OutContent=TEXT("역할군: 원거리 딜러\n");
		OutContent=TEXT("민첩한 원거리 계열 클래스이며, 다중 사격 능력을 구사합니다.\n\n");
		OutContent+=TEXT("궁수는 궁술의 달인이자, 원거리 전투의 전문가입니다.\n");
		OutContent+=TEXT("그가 쏘는 화살촉은 반드시 적의 약점을 관통합니다.\n");
		break;

	case ECharacterClassType::Mage:
		OutTitle=TEXT("마법사");
		OutContent=TEXT("역할군: 마법딜러\n");
		OutContent+=TEXT("강력한 마법 계열 클래스이며, 연쇄 번개를 구사합니다.\n\n");
		OutContent+=TEXT("마법사는 마법의 달인이자, 중거리 전투의 전문가입니다.\n");
		OutContent+=TEXT("그의 마법은 파괴적이며 잔혹하여 모든적을 파괴합니다.\n");
		break;

	default:
		OutTitle=TEXT("알 수 없음");
		OutContent=TEXT("일치하는 정보가 없습니다.");
		break;
	}
}

void URClassDetailWidget::GetBasicAttackDescription(FString& OutTitle, FString& OutContent) const
{
	switch (CurrentClassType)
	{
	case ECharacterClassType::Knight:
		OutTitle=TEXT("기본 공격");
		OutContent=TEXT("===3단 콤보 시스템===\n\n");
		OutContent+=TEXT("1번째 공격: 전방 베기\n");
		OutContent+=TEXT(" • 빠르게 수평으로 적을 벱니다.\n\n");
		OutContent+=TEXT("2번째 공격: 대각선 베기\n");
		OutContent+=TEXT(" • 강력하게 대각선으로 적을 벱니다.\n\n");
		OutContent+=TEXT("3번째 공격: 찌르기\n");
		OutContent+=TEXT(" • 치명적인 마무리일격이며, 적을 강력하게 찌릅니다.\n");
		OutContent+=TEXT(" • 마지막 공격은 더 넓은 반경으로 적을 공격합니다.\n\n");
		OutContent+=TEXT(" ※ 콤보는 1.5초후 초기화 됩니다.");
		break;

	case ECharacterClassType::Archer:
		OutTitle=TEXT("기본 공격");
		OutContent=TEXT("===다중 화살 발사===\n\n");
		OutContent+=TEXT("발사 화살 수: 5발\n");
		OutContent+=TEXT(" • 한 번에 여러 발의 화살을 동시에 발사합니다.\n\n");
		OutContent+=TEXT("확산 범위: 30도 부채꼴\n");
		OutContent+=TEXT(" • 30도 부채꼴 형태로 넓게 퍼져나가 여러명의 적들을 동시에 견제합니다.\n\n");
		OutContent+=TEXT("관통: 화살당 적 1명\n");
		OutContent+=TEXT(" • 각 화살은 하나의 적만 관통합니다.\n");
		OutContent+=TEXT(" • 가장 가까운 적부터 맞게 됩니다.\n\n");
		OutContent+=TEXT("사거리: 30m\n");
		OutContent+=TEXT(" • 30미터의 먼거리에서부터 적을 공격할 수 있습니다.\n\n");
		OutContent+=TEXT(" ※ 전방에 넓게 퍼진 적들에게 효과적입니다.");
		break;

	case ECharacterClassType::Mage:
		OutTitle=TEXT("기본 공격");
		OutContent=TEXT("═══ 연쇄 번개 ═══\n\n");
		OutContent+=TEXT("초기 사거리: 20m\n");
		OutContent+=TEXT(" • 첫 번개는 20미터 내 가장 가까운 적에게 떨어집니다.\n\n");
		OutContent+=TEXT("최대 연쇄: 9회\n");
		OutContent+=TEXT(" • 번개가 주변 적들에게 최대 9번까지 튕깁니다.\n\n");
		OutContent+=TEXT("연쇄 범위: 점프당 10m\n");
		OutContent+=TEXT(" • 튕길 때마다 10미터 내의 새로운 적을 찾아갑니다.\n\n");
		OutContent+=TEXT("작동 방식:\n");
		OutContent+=TEXT(" • 이미 맞은 적은 다시 공격하지 않습니다.\n");
		OutContent+=TEXT(" • 탐색 범위 내 가장 가까운 적을 다음 대상으로 삼습니다.\n\n");
		OutContent+=TEXT(" ※ 적들이 적당히 뭉쳐있을 때 더 많은 피해를 줍니다.");
		break;

	default:
		OutTitle=TEXT("알 수 없음");
		OutContent=TEXT("일치하는 정보가 없습니다.");
		break;
	}
}

void URClassDetailWidget::OnConfirmClicked()
{
	UE_LOG(LogTemp,Log,TEXT("확인 버튼 클릭- 닉네임 입력으로 이동!"));

	URGameInstance* GI=GetGameInstance();
	if (GI)
	{
		GI->ShowNameInputUI();
	}
}

void URClassDetailWidget::OnCancelClicked()
{
	UE_LOG(LogTemp,Warning,TEXT("취소 버튼 클릭- 클래스 선택으로 이동!"));

	URGameInstance* GI=GetGameInstance();
	if (GI)
	{
		GI->ShowClassSelectUI();
	}
}

URGameInstance* URClassDetailWidget::GetGameInstance() const
{
	return Cast<URGameInstance>(UGameplayStatics::GetGameInstance(this));
}
