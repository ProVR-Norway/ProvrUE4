// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRHelloWorldAction.generated.h"

/**
 * 
 */
UCLASS()
class PROVR_API UProVRHelloWorldAction : public UProVRActionBase
{
	GENERATED_BODY()

protected:
    virtual bool PerformAction() override;
};
