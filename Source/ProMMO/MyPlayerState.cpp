// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerState.h"
#include "ProMMO.h"
#include "Net/UnrealNetwork.h"
#include "MyGameInstance.h"


void AMyPlayerState::BeginPlay()
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BeginPlay"));
	// Call the base class  
	Super::BeginPlay();

	// clear out our currency and cubes
	//InventoryCubes = 0;
	// Resetting the currency bugs out on re-connect for some reason.  Leaving it commented out for now.
	//Currency = 0;
}


void AMyPlayerState::BroadcastChatMessage_Implementation(const FText& ChatMessageIn)
{
	// DEPRECATED - USE CHAT IN OSS INSTEAD
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BroadcastChatMessage_Implementation "));

	// Use Game Instance because it does not get erased on level change
	// Get the game instance and cast to our game instance.
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BroadcastChatMessage_Implementation - get game instance "));
	UMyGameInstance* TheGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());

	// Get the activePlayer record
	FMyActivePlayer* playerRecord = TheGameInstance->getPlayerByPlayerId(PlayerId);

	//Check to see if it was a / command, in which case we can process it here.
	FString ChatMessageInString = ChatMessageIn.ToString();
	if (ChatMessageInString.StartsWith("/"))
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BroadcastChatMessage_Implementation - Found slash command "));
		if (ChatMessageInString.StartsWith("/balance"))
		{
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BroadcastChatMessage_Implementation -  /balance  "));
			FString commandResponseSender = "SYSTEM";
			FText ChatSenderSYS = FText::FromString(commandResponseSender);

			FString playerBalance = FString::FromInt(playerRecord->currencyCurrent);
			FString commandResponse = "Balance: " + playerBalance;

			// old way
			//FString playerBalance = FString::FromInt(TheGameInstance->getPlayerBalanceByPlayerID(PlayerId));

			FText ChatMessageOut = FText::FromString(commandResponse);
			ReceiveChatMessage(ChatSenderSYS, ChatMessageOut);

		}
	}
	else
	{


		//Send the message upstream to the UETOPIA API
		//TheGameInstance->OutgoingChat(PlayerId, ChatMessageIn);

		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BroadcastChatMessage_Implementation Looping over Player controllers "));
		// Trying the chatacter controller iterator instead of playerarray
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			// old way
			/*
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BroadcastChatMessage_Implementation Casting to MyPlayerController "));
			AMyPlayerController* Controller = Cast<AMyPlayerController>(*Iterator);
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BroadcastChatMessage_Implementation Getting PlayerState "));
			AMyPlayerState* pstate = Cast<AMyPlayerState>(Controller->PlayerState);
			// Go through the activelist to grab our playerName
			UMyGameInstance* TheGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
			FString thePlayerTitle = TheGameInstance->getPlayerTitleByPlayerID(PlayerId);
			*/

			FText thePlayerName = FText::FromString(playerRecord->playerTitle);
			this->ReceiveChatMessage(thePlayerName, ChatMessageIn);

		}
	}


	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BroadcastChatMessage_Implementation Done. "));
}

bool AMyPlayerState::BroadcastChatMessage_Validate(const FText& ChatMessageIn)
{
	UE_LOG(LogTemp, Log, TEXT("Validate"));
	return true;
}

void AMyPlayerState::ReceiveChatMessage_Implementation(const FText& ChatSender, const FText& ChatMessageD)
{
	// do stuff on client here
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] ReceiveChatMessage_Implementation "));
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] ReceiveChatMessage_Implementation Broadcasting Delegate"));
	OnTextDelegate.Broadcast(ChatSender, ChatMessageD);


	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] ReceiveChatMessage_Implementation Done."));
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyPlayerState, playerTitle);
	DOREPLIFETIME(AMyPlayerState, serverTitle);
	//DOREPLIFETIME(AMyPlayerState, InventoryCubes);
	DOREPLIFETIME(AMyPlayerState, Currency);
	DOREPLIFETIME(AMyPlayerState, playerKeyId);
	//DOREPLIFETIME(AMyPlayerState, ServerPortalKeyIdsAuthorized);
	DOREPLIFETIME(AMyPlayerState, TeamId);
	DOREPLIFETIME(AMyPlayerState, Level);
	DOREPLIFETIME(AMyPlayerState, ServerLinksAuthorized);
	
	DOREPLIFETIME(AMyPlayerState, allowPickup);
	DOREPLIFETIME(AMyPlayerState, allowDrop);
}

/* handles copying properties when we do seamless travel */
void AMyPlayerState::CopyProperties(class APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(PlayerState);

	if (MyPlayerState)
	{
		//MyPlayerState->InventoryCubes = InventoryCubes;
		MyPlayerState->playerKeyId = playerKeyId;
		MyPlayerState->Currency = Currency;
		MyPlayerState->TeamId = TeamId;
		MyPlayerState->Level = Level;

		MyPlayerState->allowPickup = allowPickup;
		MyPlayerState->allowDrop = allowDrop;
	}

}
