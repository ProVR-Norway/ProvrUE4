// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRLeaveSessionAction.generated.h"

/**
 * 
 */

#define SESSION_BASE_PATH FString(TEXT("/sessions"))
UCLASS()
class PROVR_API UProVRLeaveSessionAction : public UProVRActionBase
{
	GENERATED_BODY()
	virtual EProVRActionBehavior PerformAction();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLeaveSessionCompleteDelegate, bool, Success, FString, Message);
	UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
		FOnLeaveSessionCompleteDelegate OnLeaveSessionCompleteDelegate;
};
