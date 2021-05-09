// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRInviteParticipantAction.generated.h"

UENUM(BlueprintType)
enum class EProVRInviteParticipantActionResult : uint8
{
	ENUM_OK                     UMETA(DisplayName = "User(s) invited to session successfully"),
	ENUM_UserDoesNotExists      UMETA(DisplayName = "One or more users do not exist"),
	ENUM_Unauthorized           UMETA(DisplayName = "Unauthorized. Please re-login"),
	ENUM_InternalError          UMETA(DisplayName = "Internal error"),
	ENUM_OtherError             UMETA(DisplayName = "Other error"),
};

#define SESSION_BASE_PATH FString(TEXT("/sessions"))
UCLASS()
class PROVR_API UProVRInviteParticipantAction : public UProVRActionBase
{
	GENERATED_BODY()

public:

	virtual EProVRActionBehavior PerformAction();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInviteParticipantCompleteDelegate, bool, Result, FString, Message);
	UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
	FOnInviteParticipantCompleteDelegate OnInviteParticipantCompleteDelegate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	int32 SessionId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	FString SessionName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	bool JoinSessionAfterCreation = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	TArray<FString> ParticipantsToInvite;
};
