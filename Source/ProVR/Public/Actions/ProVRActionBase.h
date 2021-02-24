// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProVRActionBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROVR_API UProVRActionBase : public UObject
{
	GENERATED_BODY()
	
protected:
	virtual bool PerformAction() { return false; }

	friend class UProVRActionManager;
};
