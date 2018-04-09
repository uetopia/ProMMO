// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "MyGameInstance.h"
//#include "MyServerPortalActor.h"
#include "MyGameState.generated.h"

/**
*
*/
UCLASS()
class PROMMO_API AMyGameState : public AGameState
{
	GENERATED_UCLASS_BODY()

		//UMyGameInstance gameInstance;

		virtual void BeginPlay();

	// Tick does not exist here
	//virtual void Tick(float DeltaSeconds) override;

	void LoadLevel();

	/* Handle to manage the timer */
	FTimerHandle ServerPortalsTimerHandle;

	//void SpawnServerPortals();

public:

	UPROPERTY(Replicated, BlueprintReadOnly)
		FMyServerLinks ServerLinks;

	UPROPERTY(Replicated, BlueprintReadOnly)
		FString serverTitle;

	UPROPERTY(Replicated, BlueprintReadOnly)
		FString MatchTitle;

	UPROPERTY(Replicated, BlueprintReadOnly)
		FMyTeamList TeamList;
	//TArray<AMyServerPortalActor*> ServerPortalActorReference;

	// Day/Night Cycle 
	// Rep notify to tell the client to load the new inventory
	UPROPERTY(ReplicatedUsing = OnRep_OnSunAngleChange)
		float sunAngle;

	// this function just calls the delegate.
	UFUNCTION()
		virtual void OnRep_OnSunAngleChange();

	float deltaSecondsElapsed = 0.0f;

	UPROPERTY(BlueprintReadWrite)
		float sunAngleMultiplier = 1.0f;

	// Server shards

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "UETOPIA")
	TArray<FMyServerShard> ServerShards;


};
