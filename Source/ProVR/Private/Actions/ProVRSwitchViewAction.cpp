// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "Actions/ProVRSwitchViewAction.h"
#include "Managers/ProVRViewManager.h"
#include "ProVRGameInstance.h"

bool UProVRSwitchViewAction::PerformAction()
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorld())
		{
			if (UProVRGameInstance* GameInstance = (UProVRGameInstance*)World->GetGameInstance())
			{
				if (UProVRViewManager* ViewManager = GameInstance->GetViewManager())
				{
					ViewManager->SwitchView(DesiredNextView);
					return true;
				}
			}
		}
	}
	return false;
}