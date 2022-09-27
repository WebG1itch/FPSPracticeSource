// Copyright Spitfire Labs 2022


#include "Networking/Beacons/ImpulseOnlineBeaconHostObject.h"

#include "OnlineBeaconHost.h"
#include "Networking/Beacons/ImpulseOnlineBeaconClient.h"

AImpulseOnlineBeaconHostObject::AImpulseOnlineBeaconHostObject()
{
	ClientBeaconActorClass = AImpulseOnlineBeaconClient::StaticClass();
	BeaconTypeName = ClientBeaconActorClass->GetName();

	HttpModule = &FHttpModule::Get();
}

void AImpulseOnlineBeaconHostObject::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
	if (Success)
	{
		ServerID = FCString::Atoi(*Response->GetContentAsString());
		UE_LOG(LogHttp, Warning, TEXT("Server ID: %d"), ServerID);
	}
}

void AImpulseOnlineBeaconHostObject::UpdateLobbyInfo(FImpulseLobbyInfo NewLobbyInfo)
{
	LobbyInfo.MapName = NewLobbyInfo.MapName;

	UpdateClientLobbyInfo();
}

void AImpulseOnlineBeaconHostObject::UpdateClientLobbyInfo()
{
	for (AOnlineBeaconClient* ClientBeacon : ClientActors)
	{
		if (AImpulseOnlineBeaconClient* Client = Cast<AImpulseOnlineBeaconClient>(ClientBeacon))
		{
			Client->Client_OnLobbyUpdated(LobbyInfo);
		}
	}
}

void AImpulseOnlineBeaconHostObject::OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection)
{
	Super::OnClientConnected(NewClientActor, ClientConnection);
	if (NewClientActor)
	{
		UE_LOG(LogBeacon, Warning, TEXT("Client Connected"));
		if (AImpulseOnlineBeaconClient* Client = Cast<AImpulseOnlineBeaconClient>(NewClientActor))
		{
			Client->Client_OnLobbyUpdated(LobbyInfo);
		}
	}
	else
	{
		UE_LOG(LogBeacon, Warning, TEXT("Client failed to connect"));
	}
}

void AImpulseOnlineBeaconHostObject::NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor)
{
	Super::NotifyClientDisconnected(LeavingClientActor);

	UE_LOG(LogBeacon, Warning, TEXT("Client has disconnected"));
}

void AImpulseOnlineBeaconHostObject::ShutDownServer()
{
	// TODO: Unregister server from master server database via web API
	DisconnectAllClients();

	// Only needed when the host plans on disconnecting entirely. This may be used for parties, but probably not needed for servers
	if (AOnlineBeaconHost* Host = Cast<AOnlineBeaconHost>(GetOwner()))
	{
		Host->UnregisterHost(BeaconTypeName);
		Host->DestroyBeacon();
	}

	if (ServerID != -1)
	{
		TSharedRef<IHttpRequest> Request = HttpModule->CreateRequest();
		
		Request->SetURL("https://localhost:44325/api/ServerManagement/" + FString::FromInt(ServerID));
		Request->SetVerb("DELETE");
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

		Request->ProcessRequest();
	}
	
}


void AImpulseOnlineBeaconHostObject::DisconnectAllClients()
{
	UE_LOG(LogBeacon, Warning, TEXT("Disconnecting all clients"));
	for (AOnlineBeaconClient* Client : ClientActors)
	{
		if (Client)
		{
			DisconnectClient(Client);
		}
	}
}

void AImpulseOnlineBeaconHostObject::DisconnectClient(AOnlineBeaconClient* ClientActor)
{
	Super::DisconnectClient(ClientActor);

	if (AImpulseOnlineBeaconClient* Client = Cast<AImpulseOnlineBeaconClient>(ClientActor))
	{
		UE_LOG(LogBeacon, Warning, TEXT("Disconnecting client %s"), *ClientActor->GetName());
		Client->Client_OnDisconnected();
	}
}

void AImpulseOnlineBeaconHostObject::BeginPlay()
{
	Super::BeginPlay();
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetNumberField("ServerID", 0);
	JsonObject->SetStringField("IPAddress", "127.0.0.1");
	JsonObject->SetStringField("ServerName", "TestName");
	JsonObject->SetStringField("MapName", "TestMap");
	JsonObject->SetNumberField("PlayerCount", 0);
	JsonObject->SetNumberField("MaxPlayers", 6);

	FString JsonString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest> Request = HttpModule->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AImpulseOnlineBeaconHostObject::OnProcessRequestComplete);

	Request->SetURL("https://localhost:44325/api/ServerManagement");
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	Request->SetContentAsString(JsonString);

	Request->ProcessRequest();
}
