// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRFindSessionAction.generated.h"

/**
 * 
 */

#define SESSION_BASE_PATH FString(TEXT("https://session-microservice-iu3tuzfidq-ez.a.run.app/sessions"))


UCLASS()
class PROVR_API UProVRFindSessionAction : public UProVRActionBase
{
	GENERATED_BODY()

public:
	virtual EProVRActionBehavior PerformAction();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFindSessionCompleteDelegate, bool, Success, FString, Message);
	UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
	FOnFindSessionCompleteDelegate OnFindSessionCompleteDelegate;
};


 
