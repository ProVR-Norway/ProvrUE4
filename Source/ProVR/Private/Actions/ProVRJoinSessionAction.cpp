// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRJoinSessionAction.h"
#include "ProVRGameInstance.h"
#include "Managers/ProVRNetworkManager.h"
#include "Kismet/GameplayStatics.h"

EProVRActionBehavior UProVRJoinSessionAction::PerformAction()
{
	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UWorld* World = GameInstance->GetWorld())
		{
			if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
			{
				FString* URLAddress_ = NetworkManager->DisplayedSessions.Find("SessionName");
				UGameplayStatics::OpenLevel(World, "35.204.67.86", false, "");

			}
		}
	}
	return EProVRActionBehavior::Synchronous;
}
