// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRLogoutAction.h"
#include "ProVRGameInstance.h"
#include "Managers/ProVRNetworkManager.h"
#include "Kismet/GameplayStatics.h"



EProVRActionBehavior UProVRLogoutAction::PerformAction()
{

	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
		{
			NetworkManager->LastPassword.Empty();
			NetworkManager->LastUsername.Empty();
			NetworkManager->CurrentAuthToken.Empty();
			if (!NetworkManager->LastPassword.IsEmpty() ||
				!NetworkManager->LastUsername.IsEmpty() ||
				!NetworkManager->CurrentAuthToken.IsEmpty())
			{
				UE_LOG(LogTemp, Warning, TEXT("Logout failed! password, username or token not deleted!"));
				return EProVRActionBehavior::Synchronous;
			}
			else
			{
				if (UWorld* World = GameInstance->GetWorld())
				{
					UGameplayStatics::OpenLevel(World, "/Game/Levels/LoginRegistrationMap", false, "");
					return EProVRActionBehavior::Synchronous;
				}
				UE_LOG(LogTemp, Warning, TEXT("Logout failed! Could not Get world!"));
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Logout failed! Something went wrong!"));
	}

	return EProVRActionBehavior::Synchronous;
	/*
	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
		{

			SessionId = NetworkManager->CurrentSession.SessionId;
			FString FullPath = FString::Printf(TEXT("/sessions/%d/participants/"), SessionId) + FGenericPlatformHttp::UrlEncode(NetworkManager->GetUsername());
			UProVRHttpRequest::DeleteWithAuthToken(FullPath, [this, GameInstance, NetworkManager](int32 HttpResponseCode, TSharedPtr<FJsonObject> HttpResponseContent)
				{
					if (HttpResponseCode == 200)
					{
						NetworkManager->LastPassword.Empty();
						NetworkManager->LastUsername.Empty();

					}
					else if (HttpResponseCode == 401)
					{
						UE_LOG(LogTemp, Warning, TEXT("Leave session action: on action complete: 401"));
						OnLogoutActionCompleteDelegate.Broadcast(false, EProVRLogoutActionResult::ENUM_Unauthorized);
					}
					else if (HttpResponseCode == 404)
					{
						UE_LOG(LogTemp, Warning, TEXT("Leave session action: on action complete: 404"));
						OnLogoutActionCompleteDelegate.Broadcast(false, EProVRLogoutActionResult::ENUM_UserOrSessionDoesNotExists);
					}
					else if (HttpResponseCode == 500)
					{
						UE_LOG(LogTemp, Warning, TEXT("Leave session action: on action complete: 500"));
						OnLogoutActionCompleteDelegate.Broadcast(false, EProVRLogoutActionResult::ENUM_InternalError);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("other error leave session"));
						OnLogoutActionCompleteDelegate.Broadcast(false, EProVRLogoutActionResult::ENUM_OtherError);
					}
					OnAsyncronousActionCompleted();
				});
		}
	}

	return EProVRActionBehavior::Asynchronous;
	*/
}
