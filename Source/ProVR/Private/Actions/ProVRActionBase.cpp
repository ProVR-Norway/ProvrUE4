// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "Actions/ProVRActionBase.h"
#include "Managers/ProVRActionManager.h"
#include "ProVRGameInstance.h"

void UProVRActionBase::OnAsyncronousActionCompleted()
{
	if (UProVRActionManager* ActionManager = UProVRGameInstance::GetActionManager())
	{
		ActionManager->OngoingActions.Remove(this);
	}
}