// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UEtopiaPersistCharacter.h"
#include "ProMMO.h"
#include "MySpawnableObject.h"
#include "MyServerPortalActor.h"
#include "MyBaseVendor.h"
#include "MyPlayerState.h"
#include "MyPlayerController.h"
#include "UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "MyGameInstance.h"


//////////////////////////////////////////////////////////////////////////
// AUEtopiaPersistCharacter

AUEtopiaPersistCharacter::AUEtopiaPersistCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Our ability system component.
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UMyAttributeSet>(TEXT("AttributeSet"));


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// set up a reference to our blueprint spawnable
	// this does not work.
	//static ConstructorHelpers::FObjectFinder<UBlueprint> InstanceOfSpawnableObject(TEXT("Blueprint'/Game/BPMySpawnableObject.BPMySpawnableObject'")); // not found
	//static ConstructorHelpers::FObjectFinder<UBlueprint> InstanceOfSpawnableObject(TEXT("Blueprint'/Game/BPMySpawnableObject.BPMySpawnableObject_C'")); // not found


	//if (InstanceOfSpawnableObject.Object) {
	//	BlueprintVar = (UClass*)InstanceOfSpawnableObject.Object->GeneratedClass;
	//}

	bReplicates = true;
	bReplicateMovement = true;

	// mouse showing default true
	bMouseShowing = true;

	// Bind to the ability interface changed delegate
	//AMyPlayerController* playerC = Cast<AMyPlayerController>(Controller);
	//FOnAbilityInterfaceChangedDelegate(this, &AUEtopiaPersistCharacter::RemapAbilities);
}


void AUEtopiaPersistCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

void AUEtopiaPersistCharacter::PossessedBy(AController* NewController)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [PossessedBy]  "));
	Super::PossessedBy(NewController);
	if (AbilitySystem)
	{
		//if (HasAuthority() && Ability)
		//{
		//AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, 0));
		//}
		AbilitySystem->InitAbilityActorInfo(this, this);
		InitAbilitySystemClient();
	}

	AMyPlayerController* playerC = Cast<AMyPlayerController>(Controller);
	playerC->GrantCachedAbilities();
	RemapAbilities();
}

void AUEtopiaPersistCharacter::InitAbilitySystemClient_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [InitAbilitySystemClient]  "));
	if (AbilitySystem)
	{
		//if (HasAuthority() && Ability)
		//{
		//AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, 0));
		//}
		AbilitySystem->InitAbilityActorInfo(this, this);
	}
}

// Called every frame -- Included in 4.7, needs to be added in 4.8
void AUEtopiaPersistCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//  make this run only on client.

	if (!IsRunningDedicatedServer()) {
		AMyBasePickup* itemSeen = GetItemFocus();

		static AMyBasePickup* oldFocus = NULL;

		oldFocus = ApplyPostProcessing(itemSeen, oldFocus);
	}



}




//////////////////////////////////////////////////////////////////////////
// Input

void AUEtopiaPersistCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AUEtopiaPersistCharacter::OnStartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &AUEtopiaPersistCharacter::OnStopFire);

	InputComponent->BindAction("Pickup", IE_Pressed, this, &AUEtopiaPersistCharacter::OnPickUp);

	InputComponent->BindAction("Travel", IE_Pressed, this, &AUEtopiaPersistCharacter::OnTravel);

	InputComponent->BindAction("Interact", IE_Pressed, this, &AUEtopiaPersistCharacter::OnInteractWithVendor);

	InputComponent->BindAction("ToggleMouse", IE_Pressed, this, &AUEtopiaPersistCharacter::OnToggleMouse);

	InputComponent->BindAxis("MoveForward", this, &AUEtopiaPersistCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AUEtopiaPersistCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AUEtopiaPersistCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AUEtopiaPersistCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AUEtopiaPersistCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AUEtopiaPersistCharacter::TouchStopped);

	// Map the "InputIDs" to the ability system
	AbilitySystem->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbiliyInputBinds("ConfirmInput", "CancelInput", "AbilityInput"));

	//AttributeSet->CreateDefaultSubobject<UMyAttributeSet>(TEXT("AttributeSet"));
	//AttributeSet = CreateDefaultSubobject<UMyAttributeSet>(TEXT("AttributeSet"));
}


void AUEtopiaPersistCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AUEtopiaPersistCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AUEtopiaPersistCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUEtopiaPersistCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUEtopiaPersistCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AUEtopiaPersistCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AUEtopiaPersistCharacter::OnStartFire()
{
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [OnStartFire] "));
	if (CanFire()) {
		FVector ShootDir = GetActorForwardVector();
		//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [OnStartFire] GetActorForwardVector"));
		FVector Origin = GetActorLocation();
		//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [OnStartFire] GetActorLocation"));
		//  Tell the server to Spawn an actor
		ServerAttemptSpawnActor();
	}

}

bool AUEtopiaPersistCharacter::CanFire()
{
	AMyPlayerState* PlayerS = Cast<AMyPlayerState>(this->PlayerState);
	AMyPlayerController* playerC = Cast<AMyPlayerController>(Controller);
	if (playerC->InventorySlots.Num() > 0) {
		return true;
	}

	/*
	if (PlayerS != NULL) {
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [CanFire] Player State found. "));
	if (PlayerS->InventoryCubes > 0) {
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [CanFire] true. "));
	return true;
	}
	else {
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [CanFire] false. "));
	return false;
	}
	}
	return false;
	*/
	return false;
	
}

void AUEtopiaPersistCharacter::OnStopFire()
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [OnStopFire] "));
	//StopWeaponFire();
}

void AUEtopiaPersistCharacter::ServerAttemptSpawnActor_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptSpawnActor_Implementation]  "));
	if (CanFire()) {
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptSpawnActor_Implementation] CanFire "));
		float placementDistance = 200.0f;
		FVector ShootDir = GetActorForwardVector();
		FVector Origin = GetActorLocation();
		FVector spawnlocationstart = ShootDir * placementDistance;
		FVector spawnlocation = spawnlocationstart + Origin;
		// Spawn the actor
		//Spawn a bomb
		FTransform const SpawnTransform(ShootDir.Rotation(), spawnlocation);
		//int32 playerID = PlayerState->PlayerId;

		UWorld* const World = GetWorld(); // get a reference to the world
		if (World) {
			// if world exists, spawn the blueprint actor
			// this does not work
			// note:  take a look at the constructor above which attempts to find the bp object.
			//AMySpawnableObject* YC = World->SpawnActor<AMySpawnableObject>(BlueprintVar, SpawnTransform);

			// get the controller
			
			AMyPlayerController* playerC = Cast<AMyPlayerController>(Controller);

			// doing it using the c++ class for now
			// hardcoding this for testing.
			//World->SpawnActor<AMyBasePickup>(playerC->InventorySlots[0].itemClass->GetDefaultObject()->GetClass(), SpawnTransform);

			UClass* LoadedActorOwnerClass;
			LoadedActorOwnerClass = LoadClassFromPath(playerC->InventorySlots[0].itemClassPath);

			World->SpawnActor<AMyBasePickup>(LoadedActorOwnerClass, SpawnTransform);

			//World->SpawnActor<AMyBasePickup>(playerC->InventorySlots[0].itemClass->GetClass(), SpawnTransform);
		}

		//AMyPlayerState* PlayerS = Cast<AMyPlayerState>(this->PlayerState);
		//if (PlayerS != NULL) {
		//	PlayerS->InventoryCubes--;
		//}

		// this spawns the C++ actor
		//GetWorld()->SpawnActor<AMySpawnableObject>(AMySpawnableObject::StaticClass(), SpawnTransform);
		//BombActor->setPlayerID(playerID);s
	}

}

bool AUEtopiaPersistCharacter::ServerAttemptSpawnActor_Validate()
{
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptSpawnActor_Validate]  "));
	return true;
}

bool AUEtopiaPersistCharacter::CanPickUp()
{
	AMyBasePickup* ObjectInFocus = GetItemFocus();
	bool doPickup = false;
	AMyPlayerController* PlayerC = Cast<AMyPlayerController>(Controller);
	AMyPlayerState* PlayerS = Cast<AMyPlayerState>(PlayerC->PlayerState);
	if (ObjectInFocus) {
		if (ObjectInFocus->bAnyoneCanPickUp)
		{
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [CanPickUp] bAnyoneCanPickUp "));
			doPickup = true;
		}
		else if (ObjectInFocus->OwnerUserKeyId == PlayerS->playerKeyId)
		{
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [CanPickUp] OwnerUserKeyId == playerKeyId "));
			if (ObjectInFocus->bOwnerCanPickUp)
			{
				UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [CanPickUp] bOwnerCanPickUp "));
				doPickup = true;
			}
			
		}
		
		if (doPickup) {
		
			if (PlayerC->AddItem(ObjectInFocus, ObjectInFocus->Quantity, true)) // CHECK ONLY!
			{
				return true;
			}
		}
	}
	return false;
}

void AUEtopiaPersistCharacter::OnPickUp()
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [OnPickUp] "));
	if (CanPickUp())
	{
		//FVector ShootDir = GetActorForwardVector();
		//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [OnPickUp] GetActorForwardVector"));
		//FVector Origin = GetActorLocation();
		//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [OnPickUp] GetActorLocation"));
		//  Tell the server to Spawn an actor
		ServerAttemptPickUp();
	}
}


void AUEtopiaPersistCharacter::ServerAttemptPickUp_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptPickUp_Implementation]  "));
	AMyBasePickup* ObjectInFocus = GetItemFocus();
	if (ObjectInFocus) {
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptPickUp_Implementation] GotItemFocus  "));
		//GetItemFocus()->GetStaticMeshComponent()->DestroyComponent();
		if (CanPickUp())
		{
			

			AMyPlayerController* PlayerC = Cast<AMyPlayerController>(Controller);
			APlayerState* thisPlayerState = PlayerC->PlayerState;
			AMyPlayerState* playerS = Cast<AMyPlayerState>(thisPlayerState);


			ObjectInFocus->OnItemPickedUp(playerS->playerKeyId);


			

			PlayerC->AddItem(ObjectInFocus, ObjectInFocus->Quantity, false);

			// TODO - rebuild widgets

			//ObjectInFocus->SetActorEnableCollision(false);
			ObjectInFocus->Destroy();
		}

	}
}

bool AUEtopiaPersistCharacter::ServerAttemptPickUp_Validate()
{
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptPickUp_Validate]  "));
	return true;
}

bool AUEtopiaPersistCharacter::CanTravel()
{
	return true;
}

void AUEtopiaPersistCharacter::OnTravel()
{
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [OnTravel] "));
	if (CanTravel())
	{
		//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [OnTravel] CanTravel  "));

		// We need to examine the server portal and determine the state that it is in.
		AMyPlayerController* myPC = Cast<AMyPlayerController>(Controller);
		AMyServerPortalActor* ObjectInFocus = GetPortalFocus();
		if (ObjectInFocus) {
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Implementation] GotItemFocus  "));

			// We keep the array with our server authorizations in the player state.  Go get it.
			APlayerState* thisPlayerState = myPC->PlayerState;
			AMyPlayerState* playerS = Cast<AMyPlayerState>(thisPlayerState);

			bool traveledAndGone = false;

			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Implementation] ServerLinksAuthorized.Num() : %d  "), playerS->ServerLinksAuthorized.Num());

			// In the case of instanced servers, the keyId is different.  TODO handle this.

			// Check to see if we are pre-authorized to use this portal.
			for (int32 b = 0; b < playerS->ServerLinksAuthorized.Num(); b++)
			{
				UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Implementation] Found Authorization KeyID  "));
				UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Implementation] ObjectInFocus->serverKeyId: %s "), *ObjectInFocus->serverKeyId);
				UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Implementation] playerS->ServerLinksAuthorized[b].targetServerKeyId: %s "), *playerS->ServerLinksAuthorized[b].targetServerKeyId);
				if (playerS->ServerLinksAuthorized[b].targetServerKeyId == ObjectInFocus->serverKeyId) {
					// If the serverKey is in the portals authorized, this player is approved to travel.
					UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Implementation] Server FOUND "));
					UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Implementation] ObjectInFocus->state: %s "), *ObjectInFocus->state);
					UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Implementation] ObjectInFocus->serverUrl: %s "), *ObjectInFocus->serverUrl);
					UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Implementation] playerS->ServerLinksAuthorized[b].hostConnectionLink: %s "), *playerS->ServerLinksAuthorized[b].hostConnectionLink);
					FString readystate = "Ready";
					FString instancedstate = "Instanced";
					if (ObjectInFocus->state == readystate) {
						UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Implementation] State is Ready - travelling "));
						ServerAttemptTravel(false);
						//traveledAndGone = true;
						//myPC->ClientTravel(ObjectInFocus->serverUrl, ETravelType::TRAVEL_Absolute);
					}
					else if (ObjectInFocus->state == instancedstate) {
						UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Implementation] State is Instanced "));
						// it is possible here to not have a travel url yet.
						// If we don't have one yet, Attempt travel again.
						if (playerS->ServerLinksAuthorized[b].hostConnectionLink != "None" )
						{
							UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Implementation] Found a hostConnectionLink "));
							ServerAttemptTravel(false);
							//traveledAndGone = true;
							//myPC->ClientTravel(playerS->ServerLinksAuthorized[b].hostConnectionLink, ETravelType::TRAVEL_Absolute);
						}
						
					}
				}
			}
			if (!traveledAndGone) {
				//  Tell the server to run a transfer.  The boolean is for test mode.
				//  True is going to tell us if the user can travel, but not actually perform the transfer request.
				// Ultimately, it will populate our ServerLinksAuthorized if we are allowed to use it!

				ServerAttemptTravel(true);
			}
		}
	}
}

void AUEtopiaPersistCharacter::ServerAttemptTravel_Implementation(bool checkOnly)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Implementation]  "));
	AMyServerPortalActor* ObjectInFocus = GetPortalFocus();
	if (ObjectInFocus) {
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Implementation] GotItemFocus  "));
		//GetItemFocus()->GetStaticMeshComponent()->DestroyComponent();
		if (CanTravel())
		{
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Implementation] CanTravel  "));
			//ObjectInFocus->SetActorEnableCollision(false);
			UWorld* const World = GetWorld();
			if (World) {
				UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Implementation] Requesting Transfer  "));
				UMyGameInstance* gameInstance = Cast<UMyGameInstance>(World->GetGameInstance());
				gameInstance->TransferPlayer(ObjectInFocus->serverKeyId, PlayerState->PlayerId, checkOnly, false);

			}
		}
	}
}

bool AUEtopiaPersistCharacter::ServerAttemptTravel_Validate(bool checkOnly)
{
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptTravel_Validate]  "));
	return true;
}

bool AUEtopiaPersistCharacter::CanInteractWithVendor()
{
	AMyBaseVendor* VendorInFocus = GetVendorFocus();
	if (VendorInFocus) {
		if (VendorInFocus->OwnerUserKeyId.IsEmpty())
		{
			return false;
		}
		if (VendorInFocus->bAnyoneCanPickUp)
		{
			return false;
		}
		if (VendorInFocus->bOwnerCanPickUp)
		{
			return false;
		}
	}
	return true;
}

void AUEtopiaPersistCharacter::OnInteractWithVendor()
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [OnInteractWithVendor] "));
	AMyBaseVendor* VendorInFocus = GetVendorFocus();
	if (VendorInFocus) {
		if (CanInteractWithVendor())
		{
			// Fire the delegate to tell BP to display the inventory interface
			AMyPlayerController* myPC = Cast<AMyPlayerController>(Controller);
			//myPC->FOnVendorInteractDisplayUI.Broadcast(VendorInFocus->VendorKeyId);
			//myPC->FOnVendorInteractChanged.Broadcast(VendorInFocus->VendorKeyId, true);
			myPC->AttemptVendorInteract(VendorInFocus);
			myPC->ServerAttemptSetVendorInteract(VendorInFocus);
		}
	}
	
}

void AUEtopiaPersistCharacter::OnToggleMouse()
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [OnToggleMouse] "));

	// Not working - prevents gameplay

	/*
	APlayerController* PC = Cast<APlayerController>(GetController());

	if (PC)
	{
		if (bMouseShowing) {
			PC->bShowMouseCursor = false;
			PC->SetInputMode(FInputModeGameOnly());
		}
		else {
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeUIOnly());
		}
		
	}

	bMouseShowing = !bMouseShowing;
	
	*/
	
	
	
	
	
}

void AUEtopiaPersistCharacter::ServerAttemptBuy_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptBuy_Implementation]  "));
	/*
	AMyStoreActor* ObjectInFocus = GetStoreFocus();
	if (ObjectInFocus) {
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptBuy_Implementation] GotStoreFocus  "));
	//GetItemFocus()->GetStaticMeshComponent()->DestroyComponent();
	if (CanBuy())
	{
	UWorld* const World = GetWorld();
	if (World) {
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptBuy_Implementation] Requesting Buy "));
	UMyGameInstance* gameInstance = Cast<UMyGameInstance>(World->GetGameInstance());
	FString itemName = "Cube";
	FString description = "You bought an in-game item:  Cube";
	FMyActivePlayer* thisPlayer = gameInstance->getPlayerByPlayerId(PlayerState->PlayerId);
	gameInstance->Purchase(thisPlayer->playerKeyId, itemName, description, 100);
	}
	}

	}
	*/
	
}

bool AUEtopiaPersistCharacter::ServerAttemptBuy_Validate()
{
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptPickUp_Validate]  "));
	return true;
}


void AUEtopiaPersistCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AUEtopiaPersistCharacter, Health);

}

class AMyBasePickup* AUEtopiaPersistCharacter::GetItemFocus() {
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [GetItemFocus]  "));
	// Attempt to use Raycasts to view an object and echo it back

	FVector Direction = this->FollowCamera->GetForwardVector();
	FVector StartTrace = GetActorLocation();
	const FVector EndTrace = StartTrace + Direction * 150.0f; //where 300 is the distance it checks

	/*
	DrawDebugLine(
		GetWorld(),
		StartTrace,
		EndTrace,
		FColor(255, 0, 0),
		false, -1, 0,
		12.333
		);
	*/

	FCollisionQueryParams TraceParams(FName(TEXT("")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, COLLISION_VIEW, TraceParams);

	AMyBasePickup* BasePickupClass = Cast<AMyBasePickup>(Hit.GetActor());

	//if (BasePickupClass->IsValidLowLevel())
	//{
	//	return BasePickupClass->GetClass();
	//}

	//return nullptr;
	return BasePickupClass;
}

AMyBasePickup* AUEtopiaPersistCharacter::ApplyPostProcessing(AMyBasePickup* itemSeen, AMyBasePickup* oldFocus) {
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ApplyPostProcessing]  "));
	if (itemSeen) {
		//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ApplyPostProcessing] itemSeen "));
		// An item is currently being looked at
		if (itemSeen == oldFocus || oldFocus == NULL) {
			//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ApplyPostProcessing] itemSeen == oldFocus || oldFocus == NULL "));
			//The item being looked at is the same as the one on the last tick

			TArray<UStaticMeshComponent*> Comps;

			itemSeen->GetComponents(Comps);
			if (Comps.Num() > 0)
			{
				UStaticMeshComponent* FoundComp = Comps[0];
				//do stuff with FoundComp
				FoundComp->SetRenderCustomDepth(true);
			}


		}
		else if (oldFocus != NULL) {
			// An item is being looked at and the old focus was not null (and not the same as the one on the last tick)
			//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ApplyPostProcessing]  oldFocus != NULL "));

			TArray<UStaticMeshComponent*> Comps;

			itemSeen->GetComponents(Comps);
			if (Comps.Num() > 0)
			{
				UStaticMeshComponent* FoundComp = Comps[0];
				//do stuff with FoundComp
				FoundComp->SetRenderCustomDepth(true);
			}

			TArray<UStaticMeshComponent*> oldComps;

			oldFocus->GetComponents(oldComps);
			if (oldComps.Num() > 0)
			{
				UStaticMeshComponent* FoundComp = oldComps[0];
				//do stuff with FoundComp
				FoundComp->SetRenderCustomDepth(false);
			}

			//UStaticMeshComponent* mesh = itemSeen->GetStaticMeshComponent();
			//itemSeen->StaticMeshComponent->SetRenderCustomDepth(true);
			//mesh->SetRenderCustomDepth(true);

			//UStaticMeshComponent* oldMesh = oldFocus->GetStaticMeshComponent();
			//oldMesh->SetRenderCustomDepth(false);
		}

		return oldFocus = itemSeen;
	}
	else {
		// No item currectly being looked at
		if (oldFocus != NULL) {
			//An item was looked at last tick but isn't being looked at anymore

			TArray<UStaticMeshComponent*> oldComps;

			oldFocus->GetComponents(oldComps);
			if (oldComps.Num() > 0)
			{
				UStaticMeshComponent* FoundComp = oldComps[0];
				//do stuff with FoundComp
				FoundComp->SetRenderCustomDepth(false);
			}


			//UStaticMeshComponent* mesh = oldFocus->GetStaticMeshComponent();
			//mesh->SetRenderCustomDepth(false);
		}

		return oldFocus = NULL;
	}

}

AMyServerPortalActor* AUEtopiaPersistCharacter::GetPortalFocus() {
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [GetPortalFocus]  "));
	// Attempt to use Raycasts to view an object and echo it back

	FVector Direction = this->FollowCamera->GetForwardVector();
	FVector StartTrace = GetActorLocation();
	const FVector EndTrace = StartTrace + Direction * 3000.0f; //where 300 is the distance it checks

															   /*
															   DrawDebugLine(
															   GetWorld(),
															   StartTrace,
															   EndTrace,
															   FColor(255, 0, 0),
															   false, -1, 0,
															   12.333
															   );
															   */

	FCollisionQueryParams TraceParams(FName(TEXT("")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, COLLISION_VIEW, TraceParams);

	// We only want portal type actors
	//GetWorld()->LineTraceSingleByObjectType(Hit, StartTrace, EndTrace, COLLISION_VIEW, TraceParams)

	return Cast<AMyServerPortalActor>(Hit.GetActor());
}


AMyBaseVendor* AUEtopiaPersistCharacter::GetVendorFocus() {
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [GetVendorFocus]  "));
	// Attempt to use Raycasts to view an object and echo it back

	FVector Direction = this->FollowCamera->GetForwardVector();
	FVector StartTrace = GetActorLocation();
	const FVector EndTrace = StartTrace + Direction * 3000.0f; //where 300 is the distance it checks

	FCollisionQueryParams TraceParams(FName(TEXT("")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, COLLISION_VIEW, TraceParams);

	// We only want portal type actors
	//GetWorld()->LineTraceSingleByObjectType(Hit, StartTrace, EndTrace, COLLISION_VIEW, TraceParams)

	return Cast<AMyBaseVendor>(Hit.GetActor());
}


FGameplayAbilitySpecHandle AUEtopiaPersistCharacter::AttemptGiveAbility( UGameplayAbility* Ability)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [AttemptGiveAbility]  "));

	FGameplayAbilitySpecHandle AbilityHandle = AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability, 1, INDEX_NONE)); // assigning abilities to -1 by default makes them not triggerable
	// For abilities to be triggerable, RemapAbilities must be run first, which looks at the ability bar setup
	return AbilityHandle;
}

bool AUEtopiaPersistCharacter::RemapAbilities_Validate()
{
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptPickUp_Validate]  "));
	return true;
}


void AUEtopiaPersistCharacter::RemapAbilities_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [RemapAbilities]  "));

	if (IsRunningDedicatedServer())
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [RemapAbilities] - Server "));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [RemapAbilities] - Client "));
	}
	AMyPlayerController* playerC = Cast<AMyPlayerController>(Controller);
	AMyPlayerState* playerS = Cast<AMyPlayerState>(playerC->PlayerState);

	FGameplayAbilitySpec* Spec;

	for (int32 abilitySlotIndex = 0; abilitySlotIndex < playerS->GrantedAbilities.Num(); abilitySlotIndex++)
	{
		Spec = AbilitySystem->FindAbilitySpecFromHandle(playerS->GrantedAbilities[abilitySlotIndex].AbilityHandle);
		if (Spec)
		{
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [RemapAbilities] classPath: %s "), *playerS->GrantedAbilities[abilitySlotIndex].classPath);
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [RemapAbilities] Spec->InputID: %d "), Spec->InputID);
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [RemapAbilities] abilitySlotIndex: %d "), abilitySlotIndex);
			Spec->InputID = abilitySlotIndex;
			AbilitySystem->MarkAbilitySpecDirty(*Spec);
		}
		
	}
	// testing - no effect
	//AbilitySystem->InitAbilityActorInfo(this, this);

}

/*
bool AUEtopiaPersistCharacter::Die_Validate(AController* Killer, AActor* DamageCauser, const FGameplayEffectSpec& KillingEffectSpec, float KillingDamageMagnitude, FVector KillingDamageNormal)
{
//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [ServerAttemptSpawnActor_Validate]  "));
return true;
}
*/


float AUEtopiaPersistCharacter::GetCooldownTimeRemaining(FGameplayAbilitySpecHandle CallingAbilityHandle, float& TotalDuration)
{
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [GetCooldownTimeRemaining]"));
	FGameplayAbilitySpec* Spec = AbilitySystem->FindAbilitySpecFromHandle(CallingAbilityHandle);
	if (Spec)
	{
		//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [GetCooldownTimeRemaining] found spec"));
		UGameplayAbility* AbilityToCheck = Spec->Ability;
		UAbilitySystemComponent* AbilityComp = Cast<IAbilitySystemInterface>(this) ? Cast<IAbilitySystemInterface>(this)->GetAbilitySystemComponent() : nullptr;

		if (AbilityToCheck && AbilityComp)
		{
			//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [GetCooldownTimeRemaining] AbilityToCheck && AbilityComp"));
			float CurrentDuration;
			AbilityToCheck->GetCooldownTimeRemainingAndDuration(CallingAbilityHandle, AbilityComp->AbilityActorInfo.Get(), CurrentDuration, TotalDuration);
			return CurrentDuration;
		}
	}
	return 0.f;

}

//void AUEtopiaPersistCharacter::Die_Implementation(AController* Killer, AActor* DamageCauser, const FGameplayEffectSpec& KillingEffectSpec, float KillingDamageMagnitude, FVector KillingDamageNormal)
void AUEtopiaPersistCharacter::Die(AController* Killer, AActor* DamageCauser, const FGameplayEffectSpec& KillingEffectSpec, float KillingDamageMagnitude, FVector KillingDamageNormal)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AUEtopiaPersistCharacter] [Die]  "));
	// start the ragdoll and cancel abilities after a very brief delay.
	GetWorldTimerManager().SetTimer(PlayDyingTimerHandle, this, &AUEtopiaPersistCharacter::PlayDying, 0.1f, false);

	AController* const DyingController = (Controller != NULL) ? Controller : Cast<AController>(GetOwner());

	bTearOff = true;
	bIsDying = true;
	
	// clear all widgets 
	AMyPlayerController* playerC = Cast<AMyPlayerController>(Controller);
	playerC->ClearHUDWidgets();

	UWorld* const World = GetWorld();
	if (World) {
		UMyGameInstance* gameInstance = Cast<UMyGameInstance>(World->GetGameInstance());
		if (gameInstance)
		{
			// report the kill
			AMyPlayerState* PlayerS = Cast<AMyPlayerState>(this->PlayerState);
			AMyPlayerState* KillerPlayerS = Cast<AMyPlayerState>(Killer->PlayerState);

			gameInstance->RecordKill(KillerPlayerS->PlayerId, PlayerS->PlayerId);
		}
	}

	DetachFromControllerPendingDestroy();

	// set it up to remove this body
	SetLifeSpan(5.0f);
}




void AUEtopiaPersistCharacter::PlayDying_Implementation()
{
	if (bIsDying)
	{
		return;
	}

	// CleanUpAbilitySystem();
	// UAbilitySystemGlobals::Get().GetGameplayCueManager()->EndGameplayCuesFor(this);


	bIsDying = true;
	bTearOff = true;
	bReplicateMovement = false;

	SetLifeSpan(5.0f);

	//DetachFromControllerPendingDestroy();

	// set it up to remove this body
	//SetLifeSpan(5.0f);

	// disable collision on the collision capsule
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	// Disable character movement
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->SetComponentTickEnabled(false);
	}

	// just go full ragdoll for now
	if (USkeletalMeshComponent* SkelMesh = GetMesh())
	{
		//static FName CollisionProfileName(TEXT("Ragdoll"));
		//PrimaryMesh->SetCollisionProfileName(CollisionProfileName);
		SkelMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SkelMesh->SetSimulatePhysics(true);

		SkelMesh->SetAllBodiesSimulatePhysics(true);
		SkelMesh->bBlendPhysics = true;
		SkelMesh->SetAnimInstanceClass(nullptr);

		// give it the impulse
		/*
		FDamageEvent& DamageEvent = LastTakeHitInfo.GetDamageEvent();
		FVector ImpulseDir;
		FHitResult Hit;
		DamageEvent.GetBestHitInfo(this, LastTakeHitInfo.PawnInstigator.Get(), Hit, ImpulseDir);
		if (DamageEvent.DamageTypeClass)
		{
		FVector const Impulse = ImpulseDir * DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>()->DamageImpulse;
		PrimaryMesh->AddImpulseAtLocation(Impulse, Hit.ImpactPoint);
		}
		// Ignore projectiles
		PrimaryMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
		*/
	}
}