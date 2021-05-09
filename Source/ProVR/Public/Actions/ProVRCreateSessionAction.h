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
UENUM(BlueprintType)
enum class EProVRCreateSessionActionResult : uint8
{
	ENUM_OK								 UMETA(DisplayName = "Session Created Successful"),
	ENUM_UserDoesNotExists			     UMETA(DisplayName = "User Does Not Exist"),
	ENUM_Unauthorized				     UMETA(DisplayName = "Unauthorized. Please re-login"),
	ENUM_InternalError                   UMETA(DisplayName = "Internal error"),
	ENUM_NoServersAvailable              UMETA(DisplayName = "No servers are currently available"),
	ENUM_SessionWithSameNameExists       UMETA(DisplayName = "Session with the same name already exists for the user"),
	ENUM_OtherError                      UMETA(DisplayName = "Other error"),
};

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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreateSessionCompleteDelegate, bool, Success, EProVRLeaveSessionActionResult, Result, int32, SessionID);
	UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
};
