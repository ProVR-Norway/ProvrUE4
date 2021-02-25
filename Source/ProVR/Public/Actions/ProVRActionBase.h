// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProVRActionBase.generated.h"

UENUM()
enum class EProVRActionBehavior : uint8
{
	Synchronous = 0,
	Asynchronous = 1
};

UCLASS(BlueprintType)
class PROVR_API UProVRActionBase : public UObject
{
	GENERATED_BODY()
	
protected:
	virtual EProVRActionBehavior PerformAction() { return EProVRActionBehavior::Synchronous; }

	friend class UProVRActionManager;

	void OnAsyncronousActionCompleted();
};
