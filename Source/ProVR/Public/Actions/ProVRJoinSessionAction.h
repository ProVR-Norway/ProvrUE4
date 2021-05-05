// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRJoinSessionAction.generated.h"

/**
 * 
 */
UCLASS()
class PROVR_API UProVRJoinSessionAction : public UProVRActionBase
{
	GENERATED_BODY()
	virtual EProVRActionBehavior PerformAction();
};
