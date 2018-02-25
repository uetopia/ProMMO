// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"
#include "ProMMO.h"
//#include "MyPawn.h"
#include "MyPlayerState.h"
#include "MyGameState.h"
#include "MyGameSession.h"
#include "MyGameInstance.h"
#include "MyPlayerController.h"
#include "UEtopiaPersistCharacter.h"
//#include "PersistCharacter.h"
#include "Blueprint/UserWidget.h"



AMyGameMode::AMyGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// set default pawn class to our flying pawn
	//DefaultPawnClass = AUEtopiaPlugCharacter::StaticClass();
	static ConstructorHelpers::FClassFinder<ACharacter> PlayerPawnBPClass(TEXT("/Game/MyUEtopiaPersistCharacter"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameMode] PlayerPawnBPClass.Class = NUL"));
	}
	//DefaultPawnClass = PlayerPawnOb.Class;

	PlayerStateClass = AMyPlayerState::StaticClass();
	GameStateClass = AMyGameState::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();

	bUseSeamlessTravel = true;

	/** Time a playerstate will stick around in an inactive state after a player logout.  seconds?
	// We don't want a player state to exist after a player has disconnected.
	// It causes duplicate results and makes it impossible to find the correct/active state */
	InactivePlayerStateLifeSpan = 0.5f;
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}

	// Tried to set the player name.  not working like this.
	/*
	APlayerController* pc = NULL;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
	pc = Iterator->Get();
	//AMyPlayerController* Mypc = Cast(pc);

	AUEtopiaCompetitiveCharacter* pawn = Cast<AUEtopiaCompetitiveCharacter>(pc->GetCharacter());
	pawn->Text->SetText(FText::FromString(pc->PlayerState->PlayerName));
	// TODO set color
	}
	*/



}

/** Returns game session class to use */
TSubclassOf<AGameSession> AMyGameMode::GetGameSessionClass() const
{
	return AMyGameSession::StaticClass();
	//return AMyGameSession::StaticClass();
}


FString AMyGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{

	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameMode] InitNewPlayer"));

	FString ErrorMessage;

	check(NewPlayerController);

	//  we need to set the platformID in the player state so we can find this player again.
	AMyPlayerState* PlayerS = Cast<AMyPlayerState>(NewPlayerController->PlayerState);
	FString Name = UGameplayStatics::ParseOption(Options, TEXT("Name"));

	if (PlayerS)
	{


		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameMode] InitNewPlayer Param1: %s"), *Name);

		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameMode] [InitNewPlayer] - Found My player state"));
		PlayerS->playerKeyId = Name;
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameMode] [InitNewPlayer] - Set platformID"));
	}


	// TODO: this should only run on dedicated server
	if (IsRunningDedicatedServer())
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] AMyGameMode::InitNewPlayer RUNNING ON DEDICATED SERVER!"));


		int32 playerId = PlayerS->PlayerId; // NewPlayerController->PlayerState->PlayerId;

		// TESTING...  This number is always 0 even with multiple players logged in...  

		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameMode] InitNewPlayer playerId: %d"), playerId);
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameMode] InitNewPlayer Options: %s"), *Options);

		/* At this point we want to be able to deny the Login, but we need to do it over http, which requires a delegate.
		// So we're just going to allow login, then kick them if the http request comes up negative
		// TODO find a better way to do this - it should probably be part of onlineSubsystem.
		*/
		UMyGameInstance* TheGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
		AMyPlayerController* playerC = Cast<AMyPlayerController>(NewPlayerController);
		

		// Register the player with the session
		// The uniqueId changed incoming and it's breaking everything else downstream
		//Tring to reconstruct it

		// Testing with this disabled..  
		// Results in the playerID not getting set correctly, and causes the deactivate routine to fail.
		// bug persists.
		const TSharedPtr<const FUniqueNetId> UniqueIdTemp = UniqueId.GetUniqueNetId();

		GameSession->RegisterPlayer(NewPlayerController, UniqueIdTemp, UGameplayStatics::HasOption(Options, TEXT("bIsFromInvite")));

		// Moving the activate routine below register allows us to use the actual UE playerID instead of 0

		playerId = PlayerS->PlayerId; // NewPlayerController->PlayerState->PlayerId;
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameMode] InitNewPlayer playerId: %d"), playerId);

		// WE need to move this...  We don't have the player access token yet.
		// TheGameInstance->ActivatePlayer(playerC, Name, playerId, UniqueId);

		// save the vars in the controller so we can run this later.
		playerC->UniqueId = UniqueId;
		playerC->playerKeyId = Name;
		playerC->playerIDTemp = playerId;

		// Init player's name
		FString InName = UGameplayStatics::ParseOption(Options, TEXT("Name")).Left(20);
		if (InName.IsEmpty())
		{
			InName = FString::Printf(TEXT("%s%i"), *DefaultPlayerName.ToString(), NewPlayerController->PlayerState->PlayerId);
		}

		ChangeName(NewPlayerController, InName, false);

		// Find a starting spot
		AActor* const StartSpot = FindPlayerStart(NewPlayerController, Portal);
		if (StartSpot != NULL)
		{
			// Set the player controller / camera in this new location
			FRotator InitialControllerRot = StartSpot->GetActorRotation();
			InitialControllerRot.Roll = 0.f;
			NewPlayerController->SetInitialLocationAndRotation(StartSpot->GetActorLocation(), InitialControllerRot);
			NewPlayerController->StartSpot = StartSpot;
		}
		else
		{
			ErrorMessage = FString::Printf(TEXT("Failed to find PlayerStart"));
		}
	}

	return ErrorMessage;
}

void AMyGameMode::Logout(AController* Exiting)
{
	// Handle users that disconnect from the server.
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameMode] [Logout] "));
	Super::Logout(Exiting);

	AMyPlayerState* ExitingPlayerState = Cast<AMyPlayerState>(Exiting->PlayerState);
	int ExitingPlayerId = ExitingPlayerState->PlayerId;

	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameMode] [Logout] ExitingPlayerId: %i"), ExitingPlayerId);

	UMyGameInstance* TheGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());

	TheGameInstance->DeActivatePlayer(ExitingPlayerId);

}

void AMyGameMode::RestartPlayer(class AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

void AMyGameMode::KilledBy(AController* Killer, AActor* DamageCauser, AController* KilledController, AActor* KilledActor, const struct FGameplayEffectSpec& KillingEffectSpec)
{
	if (KilledController)
	{
		KilledController->ChangeState(NAME_Spectating);
		RestartPlayer(KilledController);
	}
}