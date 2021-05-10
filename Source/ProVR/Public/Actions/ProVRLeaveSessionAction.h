// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRLeaveSessionAction.generated.h"

UENUM(BlueprintType)
enum class EProVRLeaveSessionActionResult : uint8
{
	ENUM_OK								 UMETA(DisplayName = "Signed URL is successfully generated"),
	ENUM_UserOrSessionDoesNotExists      UMETA(DisplayName = "User Or Session Does Not Exist"),
	ENUM_Unauthorized				     UMETA(DisplayName = "Unauthorized. Please re-login"),
	ENUM_InternalError                   UMETA(DisplayName = "Internal error"),
	ENUM_OtherError                      UMETA(DisplayName = "Other error"),
};

#define SESSION_BASE_PATH FString(TEXT("/sessions"))
UCLASS()
class PROVR_API UProVRLeaveSessionAction : public UProVRActionBase
{
public:
	GENERATED_BODY()
	virtual EProVRActionBehavior PerformAction();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	int32 SessionId;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLeaveSessionCompleteDelegate, bool, Success, EProVRLeaveSessionActionResult, Result);
	UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
	FOnLeaveSessionCompleteDelegate OnLeaveSessionCompleteDelegate;
};
