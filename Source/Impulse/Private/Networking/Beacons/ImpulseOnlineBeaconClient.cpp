// Copyright Spitfire Labs 2022


#include "Networking/Beacons/ImpulseOnlineBeaconClient.h"

AImpulseOnlineBeaconClient::AImpulseOnlineBeaconClient()
{
	Connected.AddDynamic(this, &AImpulseOnlineBeaconClient::OnConnectAttempt);
}

bool AImpulseOnlineBeaconClient::ConnectToServer(const FString& Address)
{
	FURL Destination = FURL(nullptr, *Address, ETravelType::TRAVEL_Absolute);
	Destination.Port = 7787;
	return InitClient(Destination);
}

void AImpulseOnlineBeaconClient::OnConnected()
{
	Super::OnConnected();
	UE_LOG(LogBeacon, Warning, TEXT("Client connected to beacon (client side)"));
	Connected.Broadcast(true);

}

void AImpulseOnlineBeaconClient::OnFailure()
{
	Super::OnFailure();
	UE_LOG(LogBeacon, Warning, TEXT("Client failed to connect to beacon (client side)"));
	Connected.Broadcast(false);

}

void AImpulseOnlineBeaconClient::Client_OnDisconnected_Implementation()
{
	UE_LOG(LogBeacon, Warning, TEXT("Client was disconnected"));
	Disconnected.Broadcast();
}

void AImpulseOnlineBeaconClient::Client_OnLobbyUpdated_Implementation(FImpulseLobbyInfo LobbyInfo)
{
	UpdatedLobbyInfo.Broadcast(LobbyInfo);
}

void AImpulseOnlineBeaconClient::OnConnectAttempt(bool Success)
{
	if (!Success)
	{
		DestroyBeacon();
	}
}

void AImpulseOnlineBeaconClient::LeaveLobby()
{
	DestroyBeacon();
}
