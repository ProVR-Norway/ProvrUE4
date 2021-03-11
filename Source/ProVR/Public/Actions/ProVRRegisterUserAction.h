// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRRegisterUserAction.generated.h"

UENUM(BlueprintType)
enum class EProVRRegisterUserActionResult : uint8
{
	ENUM_OK						UMETA(DisplayName = "Register OK"),
	ENUM_UserAlreadyExists		UMETA(DisplayName = "User Already Exists"),
	ENUM_InvalidEmailOrPass		UMETA(DisplayName = "Invalid Email or Password"),
	ENUM_ServerError			UMETA(DisplayName = "Server Error"),
	ENUM_OtherError				UMETA(DisplayName = "Other Errors"),
};

UCLASS()
class PROVR_API UProVRRegisterUserAction : public UProVRActionBase
{
	GENERATED_BODY()
	
public:
	virtual EProVRActionBehavior PerformAction() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	FString EmailAddress;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	FString Password;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProVRRegisterUserActionDoneDelegate, EProVRRegisterUserActionResult, Result);
	UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
	FProVRRegisterUserActionDoneDelegate OnActionComplete;
};
