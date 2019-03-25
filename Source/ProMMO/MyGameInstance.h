// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Online.h"
#include "Base64.h"
#include "math.h"
#include <string>
#include "RamaSaveLibrary.h"
#include "MyServerPortalActor.h"
#include "MyServerPortalActorParallelPriv.h"
#include "MyServerPortalActorParallelPart.h"
#include "MyServerPortalActorParallelGrou.h"
#include "MyBasePickup.h"
#include "MyTypes.h"
#include "MyGameInstance.generated.h"



USTRUCT(BlueprintType)
struct FMyActivePlayer {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 playerID;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
	//	FString platformID;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString playerKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString playerTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool authorized;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool deactivateStarted; // the deactivation process has started - don't start it again.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 roundKills;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 roundDeaths;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FString> killed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FUserEvent> events;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 rank;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 experience;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 currencyCurrent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString gamePlayerKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FUniqueNetIdRepl UniqueId;
	//UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
	AMyPlayerController* PlayerController;

	FTimerHandle GetPlayerInfoDelayHandle;
	FTimerDelegate GetPlayerInfoTimerDel;

};

USTRUCT(BlueprintType)
struct FMyActivePlayers {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyActivePlayer> ActivePlayers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString encryption;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString nonce;

};

// Matchmaker specific structs.  These are used with uetopia matchmaker functionality.
USTRUCT(BlueprintType)
struct FMyMatchPlayer {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 playerID;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
	//	FString platformID;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString userKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString userTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool joined;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 teamId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString teamKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString teamTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool win;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FString> killed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 roundKills;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 roundDeaths;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool currentRoundAlive;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 rank;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 experience;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString gamePlayerKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FUniqueNetIdRepl UniqueId;

};

USTRUCT(BlueprintType)
struct FMyMatchInfo {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyMatchPlayer> players;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 admissionFee;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString title;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 api_version;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool authorization;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString encryption;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString nonce;
};

USTRUCT(BlueprintType)
struct FMyTeamInfo {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyMatchPlayer> players;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString title;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 number;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 roundWinCount;
};

USTRUCT(BlueprintType)
struct FMyTeamList {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyTeamInfo> teams;
};


class AMyGameSession;

namespace MyGameInstanceState
{
	extern const FName None;
	extern const FName PendingInvite;
	extern const FName WelcomeScreen;
	extern const FName MainMenu;
	extern const FName MessageMenu;
	extern const FName Playing;
}


/**
*
*/
UCLASS()
class PROMMO_API UMyGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()

	// Bypasses all online subsystem calls when true, and inserts fake data
	// For normal operations, this should be false.
	// There is also a variable in PlayerController
	bool PIE_Bypass = false;

	// Populated through config file
	FString UEtopiaMode;
	FString APIURL;
	FString ServerAPIKey;  // Reusing these for matches as well, even though it's a match Key/Secret
	FString ServerAPISecret;  // Reusing these for matches as well, even though it's a match Key/Secret
	FString GameKey;
	// Populated through the get server info API call
	int32 incrementCurrency; // how much to increment for kills
	int32 serverCurrency; // how much does this server have to spend/use
	int32 minimumCurrencyRequired;
	FString ServerTitle;
	FDateTime ServerStartDateTime;
	FDateTime ServerLastRunningDateTime;


	// Populated through the get match info API call
	int32 admissionFee;
	FString MatchTitle;


	// Populated through the online subsystem
	FString ServerSessionHostAddress;
	FString ServerSessionID;



	// Use this for Dedicated Server.
	FMyActivePlayers PlayerRecord;
	FMyServerLinks ServerLinks;

	// Keep track of which map is loaded
	FString CurrentLevelEngineString;

	// Use this for matchmaker/competitive
	FMyMatchInfo MatchInfo;
	bool MatchStarted;
	int32 RoundWinsNeededToWinMatch;

	bool PerformHttpRequest(void(UMyGameInstance::*delegateCallback)(FHttpRequestPtr, FHttpResponsePtr, bool), FString APIURI, FString ArgumentString, FString AccessToken);
	bool PerformJsonHttpRequest(void(UMyGameInstance::*delegateCallback)(FHttpRequestPtr, FHttpResponsePtr, bool), FString APIURI, FString ArgumentString, FString AccessToken);

	/* Handles to manage timers */
	FTimerHandle ServerLinksTimerHandle;
	FTimerHandle RewardSpawnTimerHandle;
	FTimerHandle AttemptStartMatchTimerHandle;
	FTimerHandle SubmitReportTimerHandle;

	//*************************
	//TEMPLATE Load Obj From Path (thanks Rama!)
	//*************************
	static FORCEINLINE UClass* LoadClassFromPath(const FString& Path)
	{
		if (Path == "") return NULL;

		return StaticLoadClass(UObject::StaticClass(), NULL, *Path, NULL, LOAD_None, NULL);
	}

	//Get Path
	static FORCEINLINE FString GetClassPath(UClass* ClassPtr)
	{
		return FStringClassReference(ClassPtr).ToString();
	}

	// sharded server?
	bool bIsShardedServer;

public:

	UPROPERTY(BlueprintReadOnly)
		FMyTeamList TeamList;

	AMyGameSession* GetGameSession() const;
	virtual void Init() override;

	// TODO move this to playerController
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool StartMatchmaking(ULocalPlayer* PlayerOwner, FString MatchType);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool CancelMatchmaking(ULocalPlayer* PlayerOwner);


	/**
	*	Find an online session
	*
	*	@param UserId user that initiated the request
	*	@param bIsLAN are we searching LAN matches
	*/
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool FindSessions(ULocalPlayer* PlayerOwner, bool bLANMatch);

	// we need one for the server too without a playerowner
	bool FindSessions(bool bLANMatch);

	/** Sends the game to the specified state. */
	void GotoState(FName NewState);

	/** Obtains the initial welcome state, which can be different based on platform */
	FName GetInitialState();

	/** Sends the game to the initial startup/frontend state  */
	void GotoInitialState();

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool JoinSession(ULocalPlayer* LocalPlayer, int32 SessionIndexInSearchResults);

	bool JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult);

	/** Returns true if the game is in online mode */
	bool GetIsOnline() const { return bIsOnline; }

	/** Sets the online mode of the game */
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void SetIsOnline(bool bInIsOnline);

	/** Start the authentication flow */
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void BeginLogin(FString InType, FString InId, FString InToken);

	/** Shuts down the session, and frees any net driver */
	void CleanupSessionOnReturnToMenu();

	void RemoveExistingLocalPlayer(ULocalPlayer* ExistingPlayer);
	void RemoveSplitScreenPlayers();

	/* Get Server info is for Continuous servers.  Since we're using matchmaker on this demo, this is never called. */
	bool GetServerInfo();
	void GetServerInfoComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	/* Get Match info is for Matchmaker. */
	bool GetMatchInfo();
	void GetMatchInfoComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	void AttemptStartMatch();

	void GetServerLinks();
	//bool GetServerLinks();
	void GetServerLinksComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	void AttemptSpawnReward();



	UPROPERTY(BlueprintReadOnly)
		TArray<FMySessionSearchResult> MySessionSearchResults;

	// Holds session search results
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	/**
	* Travel to a session URL (as client) for a given session
	*
	* @param ControllerId controller initiating the session travel
	* @param SessionName name of session to travel to
	*
	* @return true if successful, false otherwise
	*/
	bool TravelToSession(int32 ControllerId, FName SessionName);

	bool RegisterNewSession(FString IncServerSessionHostAddress, FString IncServerSessionID);

	/** Returns true if the passed in local player is signed in and online */
	bool IsLocalPlayerOnline(ULocalPlayer* LocalPlayer);

	/** Returns true if owning player is online. Displays proper messaging if the user can't play */
	bool ValidatePlayerForOnlinePlay(ULocalPlayer* LocalPlayer);


	// Activate a player against the uetopia api
	// Depending on the UEtopiaMode, this has slightly different behavior
	// In dedicated mode, it finishes with ActivateRequestComplete
	// In competitive mode, it finishes with ActivateMatchPlayerRequestComplete
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool ActivatePlayer(class AMyPlayerController* NewPlayerController, FString playerKeyId, int32 playerID, const FUniqueNetIdRepl& UniqueId);
	void ActivateRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	void ActivateMatchPlayerRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	bool DeActivatePlayer(int32 playerID);
	void DeActivateRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	// Deprecated.  Use chat in OSS instead!
	bool OutgoingChat(int32 playerID, FText message);
	void OutgoingChatComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	void SubmitReport();
	void SubmitReportComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	bool SubmitMatchMakerResults();
	void SubmitMatchMakerResultsComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	UFUNCTION()
	bool GetGamePlayer(FString playerKeyId, bool bAttemptLock);
	void GetGamePlayerRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool SaveGamePlayer(FString playerKeyId, bool bAttemptUnLock);
	void SaveGamePlayerRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	void TransferPlayer(const FString& ServerKey, int32 playerID, bool checkOnly, bool toShard);
	void TransferPlayerRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	// VENDORS

	bool VendorCreate(FString userKeyId, FString VendorTypeKeyId, float coordLocationX, float coordLocationY, float coordLocationZ, float forwardVecX, float forwardVecY, float forwardVecZ);
	void VendorCreateRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	bool VendorUpdate(FString userKeyId, FString VendorKeyId, FString Title, FString Description, FString DiscordWebhook, bool bIsSelling, bool bIsBuying, bool bDisable);
	void VendorUpdateRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	bool VendorDelete(FString userKeyId, FString VendorKeyId);
	void VendorDeleteRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	bool VendorItemCreate(AMyPlayerController* playerController, FString VendorKeyId, int32 index, int32 pricePerUnit);
	void VendorItemCreateRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	bool VendorItemDelete(AMyPlayerController* playerController, FString VendorKeyId, FString VendorItemKeyId);
	void VendorItemDeleteRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	bool VendorItemBuy(AMyPlayerController* playerController, FString VendorKeyId, FString VendorItemKeyId, int32 quantity);
	void VendorItemBuyRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	bool VendorWithdraw(AMyPlayerController* playerController, FString VendorKeyId);
	void VendorWithdrawRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	bool VendorOfferDecline(AMyPlayerController* playerController, FString VendorKeyId, FString VendorItemKeyId);
	void VendorOfferDeclineRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	bool VendorItemClaim(AMyPlayerController* playerController, FString VendorKeyId, FString VendorItemKeyId);
	void VendorItemClaimRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	// Generic microtransaction functions - not vendor related
	// Use these for quest rewards, or for purchases from server store or NPCs

	bool Purchase(FString playerKeyId, FString itemName, FString description, int32 amount);
	void PurchaseRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	bool Reward(FString playerKeyId, FString itemName, FString description, int32 amount);
	void RewardRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	// Game data access
	// set these up on the backend
	bool QueryGameDataList(FString cursor);
	void QueryGameDataListRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	bool QueryGameData(FString gameDataKeyId);
	void QueryGameDataRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	void RequestBeginPlay();

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void LoadServerStateFromFile();

	// Get a player out of our custom array struct
	// PlayerID is the internal Unreal engine Integer ID
	FMyActivePlayer* getPlayerByPlayerId(int32 playerID);
	// PlayerKey is the UEtopia unique key
	FMyActivePlayer* getPlayerByPlayerKey(FString playerKeyId);

	// Delete a player and related data by Key
	void deletePlayerByPlayerKey(FString playerKeyId);

	// Get Match player
	FMyMatchPlayer* getMatchPlayerByPlayerId(int32 playerID);

	// Get a server out of our custom array struct
	FMyServerLink* getServerByKey(FString serverKey);

	// count the number of active players
	int32 getActivePlayerCount();

	// A Kill occurred.
	// Record it.
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool RecordKill(int32 killerPlayerID, int32 victimPlayerID);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool RecordEvent(int32 playerID, FString eventSummary, FString eventIcon, FString eventType);

	// Get a serverLink our of our serverlinks array by targetServerKeyId
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		FMyServerLink GetServerLinkByTargetServerKeyId(FString incomingTargetServerKeyId);

	int32 getSpawnRewardValue();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 MinimumKillsBeforeResultsSubmit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 teamCount;

	// THis is called when the last player leaves the server to notify the backend that it is safe to destroy this server.
	bool NotifyDownReady();
	void NotifyDownReadyComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	// How many items are allowed to be dropped into this level
	int32 maxPickupItemCount;

private:
	UPROPERTY(config)
		FString WelcomeScreenMap;

	UPROPERTY(config)
		FString MainMenuMap;

	FName CurrentState;
	FName PendingState;

	/** URL to travel to after pending network operations */
	FString TravelURL;

	FString _configPath = "";

	/** Whether the match is online or not */
	bool bIsOnline;

	/** If true, enable splitscreen when map starts loading */
	bool bPendingEnableSplitscreen;

	/** Whether the user has an active license to play the game */
	bool bIsLicensed;

	/** Last connection status that was passed into the HandleNetworkConnectionStatusChanged hander */
	EOnlineServerConnectionStatus::Type	CurrentConnectionStatus;

	/** Handle to various registered delegates */
	FDelegateHandle OnSearchSessionsCompleteDelegateHandle;
	//FDelegateHandle OnSearchResultsAvailableHandle;
	FDelegateHandle OnCreatePresenceSessionCompleteDelegateHandle;
	FDelegateHandle TravelLocalSessionFailureDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnEndSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	//FDelegateHandle OnMatchmakingCompleteDelegateHandle;

	/** Callback which is intended to be called upon finding sessions */
	void OnSearchSessionsComplete(bool bWasSuccessful);
	/** Callback which is intended to be called upon joining session */
	void OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result);
	/** Called after all the local players are registered in a session we're joining */
	void FinishJoinSession(EOnJoinSessionCompleteResult::Type Result);
	/** Travel directly to the named session */
	void InternalTravelToSession(const FName& SessionName);



	/** Delegate for ending a session */
	FOnEndSessionCompleteDelegate OnEndSessionCompleteDelegate;

	void HandleSessionFailure(const FUniqueNetId& NetId, ESessionFailure::Type FailureType);

	void OnEndSessionComplete(FName SessionName, bool bWasSuccessful);

	void BeginWelcomeScreenState();
	void AddNetworkFailureHandlers();
	void RemoveNetworkFailureHandlers();

	/** Called when there is an error trying to travel to a local session */
	void TravelLocalSessionFailure(UWorld *World, ETravelFailure::Type FailureType, const FString& ErrorString);

	/** Show messaging and punt to welcome screen */
	void HandleSignInChangeMessaging();

	// OSS delegates to handle
	void HandleUserLoginChanged(int32 GameUserIndex, ELoginStatus::Type PreviousLoginStatus, ELoginStatus::Type LoginStatus, const FUniqueNetId& UserId);

	void HandleUserLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);





	// We need to be aware of the HasBegunPlay function coming from gameState
	// Certain things will fail (like spawning actors), if the loadLevel has not completed.


	void SpawnServerPortals();
	TArray<AMyServerPortalActor*> ServerPortalActorReference;



	/** Whether the match is online or not */
	bool bRequestBeginPlayStarted;

	/** Variables to control the spawning of rewards */
	bool bSpawnRewardsEnabled;
	int32 SpawnRewardServerMinimumBalanceRequired;
	float SpawnRewardChance;
	int32 SpawnRewardValue;
	float SpawnRewardTimerSeconds;

	float SpawnRewardLocationXMin;
	float SpawnRewardLocationXMax;
	float SpawnRewardLocationYMin;
	float SpawnRewardLocationYMax;
	float SpawnRewardLocationZMin;
	float SpawnRewardLocationZMax;

	/** Variables to control the purchase of cubes */
	int32 CubeStoreCost;

	// Custom texture url string.
	FString customTexture;

protected:
	/** Delegate for creating a new session */
	//FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/**
	* Delegate fired when a session create request has completed
	*
	* @param SessionName the name of the session this callback is for
	* @param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	// WE probably don't need two of these, but they operate on different structs, so I'm leaving them for now.
	void CalculateNewRank(int32 WinnerPlayerIndex, int32 LoserPlayerIndex, bool penalizeLoser); // for competitive/matchmaker
	void CalculateNewRankContinuous(int32 WinnerPlayerIndex, int32 LoserPlayerIndex, bool penalizeLoser); // for continuous



};


template<typename T>
void FindAllActors(UWorld* World, TArray<T*>& Out)
{
	for (TActorIterator<AActor> It(World, T::StaticClass()); It; ++It)
	{
		T* Actor = Cast<T>(*It);
		if (Actor && !Actor->IsPendingKill())
		{
			Out.Add(Actor);
		}
	}
}
