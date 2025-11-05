// RLobbyGameState.cpp

#include "Core/Lobby/RLobbyGameState.h"
#include "Net/UnrealNetwork.h"

void ARLobbyGameState::OnRep_Party()
{
	UE_LOG(LogTemp, Log, TEXT("[RLobbyGamestate] 파티 갱신 - 현재 %d명 "), PartyMembers.Num());
}

void ARLobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARLobbyGameState, PartyMembers);
}