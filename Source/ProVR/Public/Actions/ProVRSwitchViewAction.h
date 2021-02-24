// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Actions/ProVRActionBase.h"
#include "Managers/ProVRViewManager.h"
#include "ProVRSwitchViewAction.generated.h"

UCLASS()
class PROVR_API UProVRSwitchViewAction : public UProVRActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
	EProVRView DesiredNextView = EProVRView::None;

	virtual bool PerformAction() override;
};
