// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRInviteParticipantAction.generated.h"

/**
 * 
 */
#define SESSION_BASE_PATH FString(TEXT("https://session-microservice-iu3tuzfidq-ez.a.run.app/sessions"))
UCLASS()
class PROVR_API UProVRInviteParticipantAction : public UProVRActionBase
{
	GENERATED_BODY()

		
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInviteParticipantCompleteDelegate, bool, Result, FString, Message);
	UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
		FOnInviteParticipantCompleteDelegate OnInviteParticipantComplete;
		
	virtual EProVRActionBehavior PerformAction();

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
		TArray<FString> ParticipantsToInvite;
};
