// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "Engine.h"
#include "MyTravelApprovedActor.h"
#include "MyPlayerState.generated.h"

/**
*
*/


// Instanced servers have to be handled on a per-user basis
USTRUCT(BlueprintType)
struct FMyApprovedServerLink {
	GENERATED_USTRUCT_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString targetServerKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString hostConnectionLink;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool targetInstanced;
};


//THis delegate is working.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTextDelegate, FText, chatSender, FText, chatMessage);




UCLASS()
class PROMMO_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

		virtual void BeginPlay();

	// Full Inventory moved to player controller.
	// Only the owning player needs to know about it. - not all of the players.
		//FMyInventory Inventory;

	virtual void CopyProperties(class APlayerState* PlayerState) override;


public:
	// This is the function that gets called in the widget Blueprint
	UFUNCTION(BlueprintCallable, Category = "UETOPIA", Server, Reliable, WithValidation)
		void BroadcastChatMessage(const FText& ChatMessageIn);

	// This function is called remotely by the server.
	UFUNCTION(NetMulticast, Category = "UETOPIA", Unreliable)
		void ReceiveChatMessage(const FText& ChatSender, const FText& ChatMessage);


	UPROPERTY(BlueprintAssignable)
		FTextDelegate OnTextDelegate;

	/** List of online friends
	struct FOnlineFriendsList
	{
		TArray< TSharedRef<FOnlineFriendUEtopia> > Friends;
	};
	*/

	UPROPERTY(BlueprintReadOnly)
		FString playerIdUEInternal;

	UPROPERTY(Replicated, BlueprintReadOnly)
		FString playerKeyId;
	UPROPERTY(Replicated, BlueprintReadOnly)
		FString playerTitle;
	UPROPERTY(Replicated, BlueprintReadOnly)
		FString serverTitle;

	//UPROPERTY(Replicated, BlueprintReadOnly)
	//	int32 InventoryCubes;

	UPROPERTY(Replicated, BlueprintReadOnly)
		int32 Currency;

	UPROPERTY(Replicated, BlueprintReadOnly)
		int32 Level;

	UPROPERTY(Replicated, BlueprintReadOnly)
		int32 TeamId;

	UPROPERTY(Replicated, BlueprintReadOnly)
		FString teamTitle;

	// Variables set via get game player api call

	// doubles are not supported.
	//UPROPERTY(BlueprintReadOnly)
	double savedCoordLocationX;
	//UPROPERTY(BlueprintReadOnly)
	double savedCoordLocationY;
	//UPROPERTY(BlueprintReadOnly)
	double savedCoordLocationZ;

	UPROPERTY(BlueprintReadOnly)
		FString savedZoneName;
	UPROPERTY(BlueprintReadOnly)
		FString savedZoneKey;

	UPROPERTY(BlueprintReadOnly)
		FString savedInventory;
	UPROPERTY(BlueprintReadOnly)
		FString savedEquipment;
	UPROPERTY(BlueprintReadOnly)
		FString savedAbilities;
	UPROPERTY(BlueprintReadOnly)
		FString savedInterface;

	// Authorizations for servers go in here.  These are server Key Ids.
	// This is deprecated...  Keeping a copy of the entire server link now to facilitate instances.
	//UPROPERTY(Replicated, BlueprintReadOnly)
	//	TArray<FString> ServerPortalKeyIdsAuthorized;

	// Instanced servers have a different key than the originating server template and we need to keep track of them on a per-user basis.
	// We need to keep track of the server link information and the mapping from the origin key.
	UPROPERTY(Replicated, BlueprintReadOnly)
		TArray<FMyApprovedServerLink> ServerLinksAuthorized;

	UPROPERTY(BlueprintReadOnly)
	TArray< AMyTravelApprovedActor* >  ServerTravelApprovedActors;

};
