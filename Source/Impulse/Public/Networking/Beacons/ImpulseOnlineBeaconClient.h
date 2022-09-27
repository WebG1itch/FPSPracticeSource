// Copyright Spitfire Labs 2022

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"

// TODO: Move the USTRUCT to another beacon; possibly its own header so we don't have to include a header in another header.
#include "Networking/Beacons/ImpulseOnlineBeaconHostObject.h"

#include "ImpulseOnlineBeaconClient.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectionSuccess, bool, Connected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisconnected);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLobbyUpdated, FImpulseLobbyInfo, UpdatedInfo);


UCLASS()
class IMPULSE_API AImpulseOnlineBeaconClient : public AOnlineBeaconClient
{
	GENERATED_BODY()
public:
	AImpulseOnlineBeaconClient();

protected:
	bool ConnectToServer(const FString& Address);

	virtual void OnConnected() override;
	virtual void OnFailure() override;

public:
	UFUNCTION(Client, Reliable)
	virtual void Client_OnDisconnected();

	UFUNCTION(Client, Reliable)
	void Client_OnLobbyUpdated(FImpulseLobbyInfo LobbyInfo);	
	
protected:
	UFUNCTION()
	void OnConnectAttempt(bool Success);
	
	void LeaveLobby();
	
	UPROPERTY()
	FConnectionSuccess Connected;

	UPROPERTY()
	FDisconnected Disconnected;

	UPROPERTY()
	FLobbyUpdated UpdatedLobbyInfo;
};
