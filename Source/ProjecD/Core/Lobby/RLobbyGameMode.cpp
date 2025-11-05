// RLobbyGameMode.cpp

#include "Core/Lobby/RLobbyGameMode.h"
#include "Core/Lobby/RLobbyGameState.h"
#include "Core/RGameInstance.h"

ARLobbyGameMode::ARLobbyGameMode() {}

void ARLobbyGameMode::HandleStartingNewPlayer(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer(NewPlayer);

	// 서버: 새로운 플레이어가 들어오면 PartyMembers에 추가
	if (ARLobbyGameState* GS = GetGameState<ARLobbyGameState>())
	{
		GS->PartyMembers.Add(NewPlayer->PlayerState);
		UpdatePartySatus();
	}
}

void ARLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	// 서버: 플레이어가 나가면 PartyMembers에서 제거
	if (ARLobbyGameState* GS = GetGameState<ARLobbyGameState>())
	{
		GS->PartyMembers.Remove(Exiting->PlayerState);
		UpdatePartySatus();
	}
}

void ARLobbyGameMode::UpdatePartySatus()
{
	if (ARLobbyGameState* GS = GetGameState<ARLobbyGameState>())
	{
		if (URGameInstance* GI = Cast<URGameInstance>(GetGameInstance()))
		{
			GI->SetPartyCount(GS->PartyMembers.Num());
			UE_LOG(LogTemp, Log, TEXT("[RLobbyGameMode] 파티 인원 수: %d (%s)"),
				GS->PartyMembers.Num(),
				GI->IsSoloMode() ? TEXT("솔로") : TEXT("파티"));
		}
	}
}