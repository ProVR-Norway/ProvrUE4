// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProVRPlayerController.generated.h"

/**
 * 
 */
class AProVRPawn;


UCLASS()
class PROVR_API AProVRPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	AProVRPlayerController();
	
};
