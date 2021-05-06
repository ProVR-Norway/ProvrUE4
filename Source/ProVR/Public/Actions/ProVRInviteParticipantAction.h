// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRInviteParticipantAction.generated.h"

/**
 * 
 */
UCLASS()
class PROVR_API UProVRInviteParticipantAction : public UProVRActionBase
{
	GENERATED_BODY()

		/*
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProVRLoginActionDoneDelegate, bool, Result);
	UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
		FProVRLoginActionDoneDelegate OnActionComplete;
		*/
	virtual EProVRActionBehavior PerformAction();
};
