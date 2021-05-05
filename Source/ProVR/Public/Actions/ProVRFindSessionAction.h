// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRFindSessionAction.generated.h"

/**
 * 
 */
UCLASS()
class PROVR_API UProVRFindSessionAction : public UProVRActionBase
{
	GENERATED_BODY()
public:
	virtual EProVRActionBehavior PerformAction();
};


 
