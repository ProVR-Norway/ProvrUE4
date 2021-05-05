// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRJoinSessionAction.h"
#include "ProVRGameInstance.h"


EProVRActionBehavior UProVRJoinSessionAction::PerformAction()
{
	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UWorld* World = GameInstance->GetWorld())
		{
			FString* URLAddress_ = DisplayedSessions.Find(SessionName);
			UGameplayStatics::OpenLevel(World, FName(*URLAddress_), false, "");
		}
	}
	return EProVRActionBehavior::Synchronous;
}
