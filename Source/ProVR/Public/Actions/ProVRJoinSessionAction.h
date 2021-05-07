// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRJoinSessionAction.generated.h"

/**
 * 
 */

#define SESSION_BASE_PATH FString(TEXT("/sessions"))

UCLASS()
class PROVR_API UProVRJoinSessionAction : public UProVRActionBase
{
	GENERATED_BODY()
public:
	virtual EProVRActionBehavior PerformAction();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	FString SessionName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	int32 SessionId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	bool JoinSessionAfterCreation = false;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnJoinSessionCompleteDelegate, bool, Success, FString, Message);
	UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
};
