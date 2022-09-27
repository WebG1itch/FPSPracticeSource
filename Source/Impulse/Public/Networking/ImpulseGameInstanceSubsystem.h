// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ImpulseGameInstanceSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateImpulseSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateImpulseSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartImpulseSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndImpulseSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroyImpulseSessionComplete, bool, Successful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFindImpulseSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnJoinImpulseSessionComplete, EOnJoinSessionCompleteResult::Type Result);
/**
 * 
 */
UCLASS()
class IMPULSE_API UImpulseGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UImpulseGameInstanceSubsystem();

	void CreateSession(int32 NumPublicConnections, int32 NumPrivateConnections, FString MapName, bool IsLANMatch, bool AllowInvites, bool AllowJoinInProgress, bool AllowJoinViaPresence, bool AllowJoinViaPresenceFriendsOnly, bool UsesPresence, bool IsDedicatedServer, bool ShouldAdvertise);
	void UpdateSession();
	void StartSession();
	void EndSession();
	void DestroySession();
	void FindSessions(int32 MaxSearchResults, bool IsLANQuery);
	void JoinGameSession(const FOnlineSessionSearchResult& SessionResult);

	void Login();
	
	FOnCreateImpulseSessionComplete OnCreateSessionCompleteEvent;
	FOnUpdateImpulseSessionComplete OnUpdateSessionCompleteEvent;
	FOnStartImpulseSessionComplete OnStartSessionCompleteEvent;
	FOnEndImpulseSessionComplete OnEndSessionCompleteEvent;
	FOnDestroyImpulseSessionComplete OnDestroySessionCompleteEvent;
	FOnFindImpulseSessionsComplete OnFindSessionsCompleteEvent;
	FOnJoinImpulseSessionComplete OnJoinGameSessionCompleteEvent;
	
protected:
	void OnCreateSessionCompleted(FName SessionName, bool Successful);
	void OnUpdateSessionCompleted(FName SessionName, bool Successful);
	void OnStartSessionCompleted(FName SessionName, bool Successful);
	void OnEndSessionCompleted(FName SessionName, bool Successful);
	void OnDestroySessionCompleted(FName SessionName, bool Successful);
	void OnFindSessionsCompleted(bool Successful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void OnLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	bool TryTravelToCurrentSession();
	
private:
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
	FDelegateHandle UpdateSessionCompleteDelegateHandle;

	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	FOnEndSessionCompleteDelegate EndSessionCompleteDelegate;
	FDelegateHandle EndSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
};
