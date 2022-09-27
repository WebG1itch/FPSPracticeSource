// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/ImpulseGameInstanceSubsystem.h"
#include "OnlineSubsystemUtils.h"

UImpulseGameInstanceSubsystem::UImpulseGameInstanceSubsystem()
	: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &UImpulseGameInstanceSubsystem::OnCreateSessionCompleted))
	, UpdateSessionCompleteDelegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &UImpulseGameInstanceSubsystem::OnUpdateSessionCompleted))
	, StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &UImpulseGameInstanceSubsystem::OnStartSessionCompleted))
	, EndSessionCompleteDelegate(FOnEndSessionCompleteDelegate::CreateUObject(this, &UImpulseGameInstanceSubsystem::OnEndSessionCompleted))
	, DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &UImpulseGameInstanceSubsystem::OnDestroySessionCompleted))
	, FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &UImpulseGameInstanceSubsystem::OnFindSessionsCompleted))
	, JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &UImpulseGameInstanceSubsystem::OnJoinSessionCompleted))

{ }

void UImpulseGameInstanceSubsystem::CreateSession(int32 NumPublicConnections, int32 NumPrivateConnections, FString MapName, bool IsLANMatch, bool AllowInvites, bool AllowJoinInProgress, bool AllowJoinViaPresence, bool AllowJoinViaPresenceFriendsOnly, bool UsesPresence, bool IsDedicatedServer, bool ShouldAdvertise)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}

	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		DestroySession();
	}
	
	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->NumPrivateConnections = NumPrivateConnections;
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowInvites = AllowInvites;
	LastSessionSettings->bAllowJoinInProgress = AllowJoinInProgress;
	LastSessionSettings->bAllowJoinViaPresence = AllowJoinViaPresence;
	LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = AllowJoinViaPresenceFriendsOnly;
	LastSessionSettings->bIsDedicated = IsDedicatedServer;
	LastSessionSettings->bUsesPresence = UsesPresence;
	LastSessionSettings->bIsLANMatch = IsLANMatch;
	LastSessionSettings->bShouldAdvertise = ShouldAdvertise;
	LastSessionSettings->bUseLobbiesIfAvailable = false;

	LastSessionSettings->Set(SEARCH_KEYWORDS, FString("ImpulseTestServer"), EOnlineDataAdvertisementType::ViaOnlineService);
	//LastSessionSettings->Set(SETTING_GAMEMODE, FString("DeathMatch"), EOnlineDataAdvertisementType::ViaOnlineService);
	//LastSessionSettings->Set(SETTING_MAPNAME, MapName, EOnlineDataAdvertisementType::ViaOnlineService);
	//LastSessionSettings->Set(SETTING_MATCHING_HOPPER, FString("Deathmatch"), EOnlineDataAdvertisementType::DontAdvertise);
	//LastSessionSettings->Set(SETTING_MATCHING_TIMEOUT, 120, EOnlineDataAdvertisementType::ViaOnlineService);
	//LastSessionSettings->Set(SETTING_SESSION_TEMPLATE_NAME, FString("ImpulseTestGameSession"), EOnlineDataAdvertisementType::DontAdvertise);
	//LastSessionSettings->Set(SETTING_ALLOWBROADCASTING, true, EOnlineDataAdvertisementType::ViaOnlineService);
	
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	
	if(!SessionInterface->CreateSession(0, NAME_GameSession, *LastSessionSettings))
	{
		UE_LOG(LogTemp, Warning, TEXT("Something went wrong while creating a session"));

		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		OnCreateSessionCompleteEvent.Broadcast(false);
	}
	
}

void UImpulseGameInstanceSubsystem::UpdateSession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnUpdateSessionCompleteEvent.Broadcast(false);
		return;
	}

	TSharedPtr<FOnlineSessionSettings> UpdatedSessionSettings = MakeShareable(new FOnlineSessionSettings(*LastSessionSettings));
	//UpdatedSessionSettings->Set(SETTING_MAPNAME, FString("Updated Level Name"), EOnlineDataAdvertisementType::ViaOnlineService);

	UpdateSessionCompleteDelegateHandle = SessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegate);

	if (!SessionInterface->UpdateSession(NAME_Game, *UpdatedSessionSettings))
	{
		SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);
		OnUpdateSessionCompleteEvent.Broadcast(false);
	}
	else
	{
		LastSessionSettings = UpdatedSessionSettings;
	}
}

void UImpulseGameInstanceSubsystem::StartSession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnStartSessionCompleteEvent.Broadcast(false);
		return;
	}

	StartSessionCompleteDelegateHandle = SessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);

	if (!SessionInterface->StartSession(NAME_GameSession))
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
		OnStartSessionCompleteEvent.Broadcast(false);
	}
}

void UImpulseGameInstanceSubsystem::EndSession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnEndSessionCompleteEvent.Broadcast(false);
		return;
	}

	EndSessionCompleteDelegateHandle = SessionInterface->AddOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegate);

	if(!SessionInterface->EndSession(NAME_GameSession))
	{
		SessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);
		OnEndSessionCompleteEvent.Broadcast(false);
	}
}

void UImpulseGameInstanceSubsystem::DestroySession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnDestroySessionCompleteEvent.Broadcast(false);
		return;
	}

	DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	if (!SessionInterface->DestroySession(NAME_GameSession))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);

		OnDestroySessionCompleteEvent.Broadcast(false);
	}
}

void UImpulseGameInstanceSubsystem::FindSessions(int32 MaxSearchResults, bool IsLANQuery)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Begin find sessions"));

	FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IsLANQuery;
	//LastSessionSearch->QuerySettings.Set(SEARCH_KEYWORDS, FString("ImpulseTestServer"), EOnlineComparisonOp::Equals);
	//LastSessionSearch->QuerySettings.Set(SEARCH_DEDICATED_ONLY, true, EOnlineComparisonOp::Equals);

	UE_LOG(LogTemp, Warning, TEXT("end session finding rules"));
	
	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	/**localPlayer->GetPreferredUniqueNetId()*/
	SessionInterface->FindSessions(0, LastSessionSearch.ToSharedRef());

	if (!LastSessionSearch)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

void UImpulseGameInstanceSubsystem::JoinGameSession(const FOnlineSessionSearchResult& SessionResult)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnJoinGameSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	UE_LOG(LogTemp, Warning, TEXT("Attempting to join session"));

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		OnJoinGameSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UImpulseGameInstanceSubsystem::Login()
{
	if (IsRunningDedicatedServer())
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempting to login, but is running on server"));

		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Attempting to login"));

	if(IOnlineIdentityPtr Identity = Online::GetIdentityInterface())
	{
		FOnlineAccountCredentials Credentials;
		Credentials.Id = FString();
		Credentials.Token = FString();
		Credentials.Type = FString("accountportal");

		Identity->OnLoginCompleteDelegates->AddUObject(this, &UImpulseGameInstanceSubsystem::OnLoginCompleted);
		Identity->Login(0, Credentials);
	}
}

void UImpulseGameInstanceSubsystem::OnCreateSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	OnCreateSessionCompleteEvent.Broadcast(Successful);

	if (Successful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session created : true"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session created : false"));
	}
}

void UImpulseGameInstanceSubsystem::OnUpdateSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);
	}

	OnUpdateSessionCompleteEvent.Broadcast(Successful);
}

void UImpulseGameInstanceSubsystem::OnStartSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
	}

	OnStartSessionCompleteEvent.Broadcast(Successful);
}

void UImpulseGameInstanceSubsystem::OnEndSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);
	}

	OnEndSessionCompleteEvent.Broadcast(Successful);
}

void UImpulseGameInstanceSubsystem::OnDestroySessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}

	OnDestroySessionCompleteEvent.Broadcast(Successful);
}

void UImpulseGameInstanceSubsystem::OnFindSessionsCompleted(bool Successful)
{
	UE_LOG(LogTemp, Warning, TEXT("Find session complete delegate fired"));

	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	UE_LOG(LogTemp, Warning, TEXT("find sessions successful %i"), LastSessionSearch->SearchResults.Num());

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), Successful);
		return;
	}

	for (FOnlineSessionSearchResult Result : LastSessionSearch->SearchResults)
	{
		if (Result.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("possibly found impulse session"));
			//JoinGameSession(Result);
		}
	}
	
	OnFindSessionsCompleteEvent.Broadcast(LastSessionSearch->SearchResults, Successful);
}

void UImpulseGameInstanceSubsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}
	UE_LOG(LogTemp, Warning, TEXT("join sessions successful"));

	OnJoinGameSessionCompleteEvent.Broadcast(Result);
	
	if (TryTravelToCurrentSession())
	{
		UE_LOG(LogTemp, Warning, TEXT("travel sessions successful"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("travel sessions unsuccessful"));

	}
}
void UImpulseGameInstanceSubsystem::OnLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Logged in: %d"), bWasSuccessful);

	if(IOnlineIdentityPtr Identity = Online::GetIdentityInterface())
	{
		Identity->ClearOnLoginCompleteDelegates(0, this);
	}
}

bool UImpulseGameInstanceSubsystem::TryTravelToCurrentSession()
{

	UE_LOG(LogTemp, Warning, TEXT("attempting to travel"));

	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		return false;
	}

	FString ConnectString;
	if (!SessionInterface->GetResolvedConnectString(NAME_GameSession, ConnectString))
	{
		return false;
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->ClientTravel(ConnectString, TRAVEL_Absolute);

	return true;
}
