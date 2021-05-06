// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRCreateSessionAction.generated.h"

/**
 * 
 */

#define SESSION_BASE_PATH FString(TEXT("https://session-microservice-iu3tuzfidq-ez.a.run.app/sessions"))

UCLASS()
class PROVR_API UProVRCreateSessionAction : public UProVRActionBase
{
	GENERATED_BODY()

	UProVRCreateSessionAction();

public:
	virtual EProVRActionBehavior PerformAction();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	FString MapName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	FString SessionName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	int64 MaxPlayers;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreateSessionCompleteDelegate, bool, Success, FString, Message);
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
};
