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

UENUM(BlueprintType)
enum class EProVRJoinSessionActionResult : uint8
{
	ENUM_OK								 UMETA(DisplayName = "Pariticpant is able to join"),
	ENUM_UserOrSessionDoesNotExists		 UMETA(DisplayName = "User or Session Does Not Exist"),
	ENUM_Unauthorized				     UMETA(DisplayName = "Unauthorized. Please re-login"),
	ENUM_InternalError                   UMETA(DisplayName = "Internal error"),
	ENUM_OtherError                      UMETA(DisplayName = "Other error"),
};


UCLASS()
class PROVR_API UProVRJoinSessionAction : public UProVRActionBase
{
	GENERATED_BODY()
public:
	virtual EProVRActionBehavior PerformAction();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	int32 SessionId;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnJoinSessionCompleteDelegate, bool, Success, EProVRJoinSessionActionResult, Result);
	UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
};
