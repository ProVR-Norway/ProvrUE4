// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRGoogleHttpRequestAction.generated.h"

/**
 * 
 */
UCLASS()
class PROVR_API UProVRGoogleHttpRequestAction : public UProVRActionBase
{
	GENERATED_BODY()
	
public:
	virtual EProVRActionBehavior PerformAction() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProVRRequestDoneDelegate, bool, Success, FString, Content);
	UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
	FProVRRequestDoneDelegate OnActionComplete;
};
