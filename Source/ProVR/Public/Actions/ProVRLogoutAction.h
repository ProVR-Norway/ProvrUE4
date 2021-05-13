// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRLogoutAction.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EProVRLogoutActionResult : uint8
{
	ENUM_OK						UMETA(DisplayName = "Login OK"),
	ENUM_UserDoesNotExists		UMETA(DisplayName = "User Does Not Exists"),
	ENUM_InvalidUsernameOrPass	UMETA(DisplayName = "Invalid Username or Password"),
	ENUM_ServerError			UMETA(DisplayName = "Server Error"),
	ENUM_OtherError				UMETA(DisplayName = "Other Errors"),
};

UCLASS()
class PROVR_API UProVRLogoutAction : public UProVRActionBase
{
	GENERATED_BODY()
	
	virtual EProVRActionBehavior PerformAction() override;

public:

};
