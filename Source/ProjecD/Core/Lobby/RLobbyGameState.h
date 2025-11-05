// RLobbyGameState.h

#pragma once

#include "CoreMinimal.h"
#include "Core/Base/RGameStateBase.h"
#include "RLobbyGameState.generated.h"

UCLASS()
class PROJECD_API ARLobbyGameState : public ARGameStateBase
{
	GENERATED_BODY()

public:
	// 현제 파티에 포함된 플레이어 목록(자동 복제)
	UPROPERTY(ReplicatedUsing=OnRep_Party)
	TArray<APlayerState*> PartyMembers;

	//파티 정보 갱신될때 클라이언트에서 호출
	UFUNCTION()
	void OnRep_Party();

	//복제 설정
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
