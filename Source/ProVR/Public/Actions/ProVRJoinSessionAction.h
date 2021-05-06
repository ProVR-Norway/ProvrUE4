// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRJoinSessionAction.generated.h"

/**
 * 
 */

#define SESSION_BASE_PATH FString(TEXT("https://session-microservice-iu3tuzfidq-ez.a.run.app/sessions"))

UCLASS()
class PROVR_API UProVRJoinSessionAction : public UProVRActionBase
{
	GENERATED_BODY()
	virtual EProVRActionBehavior PerformAction();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnJoinSessionCompleteDelegate, bool, Success);
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
};
