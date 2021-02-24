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

		//If PerformAction returns false, it is an async. process; if true; we can remove it from the queue directly
		if (!ActionToExecute->PerformAction())
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