// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Managers/ProVRManagerBase.h"
#include "ProVRActionManager.generated.h"

/**
 * 
 */
UCLASS()
class PROVR_API UProVRActionManager : public UProVRManagerBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<class UProVRActionBase*> ActionQueue;

	UPROPERTY()
	TArray<class UProVRActionBase*> OngoingActions;

	friend class UProVRActionBase;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ProVR|Actions")
	void QueueAction(class UProVRActionBase* Action);
};
