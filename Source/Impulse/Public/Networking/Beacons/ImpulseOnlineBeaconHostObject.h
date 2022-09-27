// Copyright Spitfire Labs 2022

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconHostObject.h"
#include "Http.h"
#include "Interfaces/IHttpRequest.h"
#include "ImpulseOnlineBeaconHostObject.generated.h"

// TODO: Perhaps move this to a party / custom game beacon?
USTRUCT()
struct FImpulseLobbyInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapName = FString("");
	
	UPROPERTY()
	TArray<FString> PlayerList;
};

UCLASS()
class IMPULSE_API AImpulseOnlineBeaconHostObject : public AOnlineBeaconHostObject
{
	GENERATED_BODY()
public:
	AImpulseOnlineBeaconHostObject();

protected:
	
	FHttpModule* HttpModule;
	int ServerID;
	void OnProcessRequestComplete(/*TSharedPtr<IHttpRequest, ESPMode::ThreadSafe>*/ FHttpRequestPtr Request, /*TSharedPtr<IHttpRequest, ESPMode::ThreadSafe>*/ FHttpResponsePtr Response, bool Success);

protected:
	
	FImpulseLobbyInfo LobbyInfo;

	void UpdateLobbyInfo(FImpulseLobbyInfo NewLobbyInfo);

	void UpdateClientLobbyInfo();
	
	virtual void OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection) override;
	virtual void NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor) override;

	void ShutDownServer();
	
	void DisconnectAllClients();
	
	virtual void DisconnectClient(AOnlineBeaconClient* ClientActor) override;

	virtual void BeginPlay() override;

	void OnProcessRequestComplete();
};
