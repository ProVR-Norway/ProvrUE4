// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "Actions/ProVRSwitchViewAction.h"
#include "Managers/ProVRViewManager.h"
#include "ProVRGameInstance.h"

EProVRActionBehavior UProVRSwitchViewAction::PerformAction()
{
	if (UProVRViewManager* ViewManager = UProVRGameInstance::GetViewManager())
	{
		ViewManager->SwitchView(DesiredNextView);
	}
	return EProVRActionBehavior::Synchronous;
}