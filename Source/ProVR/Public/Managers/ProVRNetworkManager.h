// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Managers/ProVRManagerBase.h"
#include "ProVRNetworkManager.generated.h"

/**
 * 
 */
UCLASS()
class PROVR_API UProVRNetworkManager : public UProVRManagerBase
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;
};
