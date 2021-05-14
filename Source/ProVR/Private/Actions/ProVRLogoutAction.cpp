// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRLogoutAction.h"
#include "ProVRGameInstance.h"
#include "Managers/ProVRNetworkManager.h"
#include "Kismet/GameplayStatics.h"


EProVRActionBehavior UProVRLogoutAction::PerformAction()
{
	Complete.BindUFunction(this, "OnLogoutEventComplete");
	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
		{
			if (NetworkManager->Logout(Complete))
			{
				return EProVRActionBehavior::Asynchronous;
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Logout failed! Something went wrong!"));
	OnLogoutActionCompleteDelegate.Broadcast(false);
	return EProVRActionBehavior::Synchronous;
}


void UProVRLogoutAction::OnLogoutEventComplete(bool Success)
{
	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UWorld* World = GameInstance->GetWorld())
		{
			UGameplayStatics::OpenLevel(World, "/Game/Levels/LoginRegistrationMap", false, "");
			OnLogoutActionCompleteDelegate.Broadcast(true);
			OnAsyncronousActionCompleted();
			return;
		}
	}
	OnLogoutActionCompleteDelegate.Broadcast(false);
	UE_LOG(LogTemp, Warning, TEXT("Asynchronius Action Complete Function running on logout action"));
	OnAsyncronousActionCompleted();
}
