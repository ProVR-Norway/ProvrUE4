// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "Managers/ProVRActionManager.h"
#include "Actions/ProVRActionBase.h"

void UProVRActionManager::Tick(float DeltaTime)
{
	UProVRManagerBase::Tick(DeltaTime);

	if (ActionQueue.Num() > 0)
	{
		UProVRActionBase* ActionToExecute = ActionQueue[0];

		ActionQueue.RemoveAt(0);

		if (ActionToExecute->PerformAction() == EProVRActionBehavior::Asynchronous)
		{
			OngoingActions.Add(ActionToExecute);
		}
	}
}

void UProVRActionManager::QueueAction(UProVRActionBase* Action)
{
	if (Action && Action->IsValidLowLevel() && !Action->IsPendingKill())
	{
		ActionQueue.Add(Action);
	}
}