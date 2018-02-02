// Fill out your copyright notice in the Description page of Project Settings.

#include "MyStoreActor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "ProMMO.h"




// Sets default values
AMyStoreActor::AMyStoreActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"));
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	GetStaticMeshComponent()->SetStaticMesh(SphereVisualAsset.Object);
	GetStaticMeshComponent()->SetRelativeLocation(FVector(1.0f, 1.0f, -3.0f));
	RootComponent = GetStaticMeshComponent();

	if (!IsRunningDedicatedServer()) {
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyStoreActor] construct - Not a Dedicated server."));
		// For some reason the dedicated server can't load the material, but it does not need it.

		static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("Material'/Game/StoreMaterial.StoreMaterial'"));
		// Material'/Game/GlowMaterial.GlowMaterial'
		if (MatFinder.Succeeded())
		{
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyStoreActor] [Construct]  "));
			Material = MatFinder.Object;
			GetStaticMeshComponent()->SetMaterial(0, Material);
		}


	}
	//bReplicates = true;
	//bReplicateMovement = true;
}
