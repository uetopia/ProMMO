// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "MyStoreActor.generated.h"

/**
 *
 */
UCLASS()
class PROMMO_API AMyStoreActor : public AStaticMeshActor
{
	GENERATED_UCLASS_BODY()

private:
	UPROPERTY(EditAnywhere)
	class UMaterial* Material;

	UPROPERTY(EditAnywhere)
	class UMaterialInstanceDynamic* MaterialInstance;




};
