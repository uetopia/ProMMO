// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

/**
*
*/
UCLASS()
class PROMMO_API AMyGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()



protected:
	/** Returns game session class to use */
	virtual TSubclassOf<AGameSession> GetGameSessionClass() const override;

	/**
	* Customize incoming player based on URL options
	*
	* @param NewPlayerController player logging in
	* @param UniqueId unique id for this player
	* @param Options URL options that came at login
	*
	*/
	virtual FString InitNewPlayer(class APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT(""));

	/** The Widget class to use for our HUD screen */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UEtopia", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	/** The instance of the HUD */
	UPROPERTY()
	class UUserWidget* CurrentWidget;


public:
	//AMyGameMode();
	//virtual void PreLogin(const FString& Options, const FString& Address, const TSharedPtr<const FUniqueNetId>& UniqueId, FString& ErrorMessage) override;

	void Logout(AController* Exiting);

	virtual void BeginPlay() override;


};
