// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRCreateSessionAction.generated.h"

/**
 * 
 */

#define SESSION_BASE_PATH FString(TEXT("/sessions"))

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
	int32 MaxPlayers;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreateSessionCompleteDelegate, bool, Success, FString, Message, int32, SessionID);
	UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
};
