// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRLogoutAction.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FProVRLogoutActionComplete, bool, Success);

UCLASS()
class PROVR_API UProVRLogoutAction : public UProVRActionBase
{
	GENERATED_BODY()
	
	virtual EProVRActionBehavior PerformAction() override;

public:


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProVRLogoutActionDoneDelegate, bool, Success);
	UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
	FProVRLogoutActionDoneDelegate OnLogoutActionCompleteDelegate;

	UFUNCTION()
	void OnLogoutEventComplete(bool Success);

	FProVRLogoutActionComplete Complete;
};
