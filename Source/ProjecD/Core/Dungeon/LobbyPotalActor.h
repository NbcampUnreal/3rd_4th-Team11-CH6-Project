// LobbyPotalActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LobbyPotalActor.generated.h"

UCLASS()
class PROJECD_API ALobbyPotalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ALobbyPotalActor();

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(visibleAnywhere)
	TObjectPtr<UStaticMeshComponent> PortalMesh;
};
