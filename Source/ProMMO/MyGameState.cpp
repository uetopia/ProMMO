// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameState.h"
#include "ProMMO.h"
#include "Net/UnrealNetwork.h"
//#include "MyServerPortalActor.h"



AMyGameState::AMyGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameState] CONSTRUCT"));

	//GetWorld()->GetTimerManager().SetTimer(ServerPortalsTimerHandle, this, &AMyGameState::SpawnServerPortals, 20.0f, true);
	ServerShards.Empty();

}

void AMyGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyGameState, serverTitle);
	DOREPLIFETIME(AMyGameState, ServerLinks);
	DOREPLIFETIME(AMyGameState, TeamList);
	DOREPLIFETIME(AMyGameState, ServerShards);

}

/*

Tick does not exist here.
void AMyGameState::Tick(float DeltaTime)
{
Super::Tick(DeltaTime);

UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameState] tick"));

// we want to update the sun angle, but we only need to do it like once a minute or even less.
deltaSecondsElapsed = deltaSecondsElapsed + DeltaTime;
if (deltaSecondsElapsed > 60.0f)
{
UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameState] Updating Sun Angle."));
if (sunAngle >= 360.0f)
{
sunAngle = sunAngle - 360.0f + (0.25f * sunAngleMultiplier);
}
else
{
sunAngle = sunAngle + (0.25f * sunAngleMultiplier);
}
deltaSecondsElapsed = deltaSecondsElapsed - 60.0f;
}

}
*/


void AMyGameState::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (IsRunningDedicatedServer()) {
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameState] Dedicated server found."));

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &AMyGameState::LoadLevel, 2.0f, false);
	}
}


void AMyGameState::LoadLevel()
{

	if (IsRunningDedicatedServer()) {
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameState] LoadLevel - Dedicated server found."));

		// Uncomment this if you want to use Rama Save System
		// Or implement your own save functionality here.
		// Load our old data
		/*
		bool FileIOSuccess;
		TArray<FString> StreamingLevelsStates;
		FString FileName = "serversavedata.dat";

		URamaSaveLibrary::RamaSave_LoadFromFile(GetWorld(), FileIOSuccess, FileName);

		//URamaSaveLibrary::RamaSave_LoadFromFile(GetWorld(), FileIOSuccess, FileName, true, true, true, "PersistentLevel");
		//URamaSaveLibrary::RamaSave_LoadStreamingStateFromFile(GetWorld(), FileIOSuccess, FileName, StreamingLevelsStates);
		if (FileIOSuccess) {
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameState] LoadLevel File IO Success."));
		}
		else {
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameState] LoadLevel File IO FAIL."));
		}
		*/
		

		// This breaks the current session - don't do this.
		//if (IsRunningDedicatedServer()) {
		//	gameInstance->FindSessions(false);
		//}
		//SpawnServerPortals();
	}
}

void AMyGameState::OnRep_OnSunAngleChange()
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameState] OnRep_OnSunAngleChange."));
}