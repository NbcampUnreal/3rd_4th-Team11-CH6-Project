//LobbyPotalActor.cpp

#include "Core/Dungeon/LobbyPotalActor.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"


ALobbyPotalActor::ALobbyPotalActor()
{
	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	RootComponent = PortalMesh;

	PortalMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void ALobbyPotalActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("플레이어 포탈 진입 -> 로비 이동"));
	
	if (UWorld* World = GetWorld())
	{
		World->ServerTravel(TEXT("/Game/Maps/Lobby_Level?listen"), true);
	}
}