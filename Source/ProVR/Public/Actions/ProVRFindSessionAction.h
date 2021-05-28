// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRFindSessionAction.generated.h"

/**
 * 
 */

#define SESSION_BASE_PATH FString(TEXT("/sessions"))

UENUM(BlueprintType)
enum class EProVRFindSessionActionResult : uint8
{
	ENUM_OK								 UMETA(DisplayName = "All relevant sessions are returned"),
	ENUM_UserDoesNotExists				 UMETA(DisplayName = "User Does Not Exist"),
	ENUM_Unauthorized				     UMETA(DisplayName = "Unauthorized. Please re-login"),
	ENUM_InternalError                   UMETA(DisplayName = "Internal error"),
	ENUM_OtherError                      UMETA(DisplayName = "Other error"),
};


UCLASS()
class PROVR_API UProVRFindSessionAction : public UProVRActionBase
{
	GENERATED_BODY()

public:
	virtual EProVRActionBehavior PerformAction();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFindSessionCompleteDelegate, bool, Success, EProVRFindSessionActionResult, Message);
	UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
	FOnFindSessionCompleteDelegate OnFindSessionCompleteDelegate;
};


 
