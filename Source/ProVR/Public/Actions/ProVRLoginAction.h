// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRLoginAction.generated.h"

UENUM(BlueprintType)
enum class EProVRLoginActionResult : uint8
{
	ENUM_OK						UMETA(DisplayName = "Login OK"),
	ENUM_UserDoesNotExists		UMETA(DisplayName = "User Does Not Exists"),
	ENUM_InvalidEmailOrPass		UMETA(DisplayName = "Invalid Email or Password"),
	ENUM_ServerError			UMETA(DisplayName = "Server Error"),
	ENUM_OtherError				UMETA(DisplayName = "Other Errors"),
};

UCLASS()
class PROVR_API UProVRLoginAction : public UProVRActionBase
{
	GENERATED_BODY()
	
public:
	virtual EProVRActionBehavior PerformAction() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	FString EmailAddress;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	FString Password;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProVRLoginActionDoneDelegate, EProVRLoginActionResult, Result);
	UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
	FProVRLoginActionDoneDelegate OnActionComplete;
};
