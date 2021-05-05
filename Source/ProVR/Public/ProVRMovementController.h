// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "ProVRMovementController.generated.h"

/**
 * 
 */
UCLASS()
class PROVR_API UProVRMovementController : public UPawnMovementComponent
{
	GENERATED_BODY()
public:
	UProVRMovementController();
	~UProVRMovementController();
};
