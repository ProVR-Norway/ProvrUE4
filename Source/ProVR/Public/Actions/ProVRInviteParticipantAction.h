// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRInviteParticipantAction.generated.h"

/**
 * 
 */
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
