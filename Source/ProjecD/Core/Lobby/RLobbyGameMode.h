// RLobbyGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "Core/Base/RGameModeBase.h"
#include "RLobbyGameMode.generated.h"

UCLASS()
class PROJECD_API ARLobbyGameMode : public ARGameModeBase
{
	GENERATED_BODY()

public:
	ARLobbyGameMode();

protected:
	// 새로운 플레이어가 접속했을때 호출
	virtual void HandleStartingNewPlayer(APlayerController* NewPlayer);

	//플레이어가 나갔을때
	virtual void Logout(AController* Exiting) override;

private:
	//파티 인원수 갱신
	void UpdatePartySatus();
};
