// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRLeaveSessionAction.h"
#include "Network/ProVRHttpRequest.h"
#include "ProVRGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ProVRNetworkManager.h"
#include "GenericPlatform/GenericPlatformHttp.h"
       
EProVRActionBehavior UProVRLeaveSessionAction::PerformAction()
{
	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
		{
			if (!NetworkManager->bInASession)
			{
				OnLeaveSessionCompleteDelegate.Broadcast(false, EProVRLeaveSessionActionResult::ENUM_IsNotInSession);
				return EProVRActionBehavior::Synchronous;
			}
			SessionId = NetworkManager->CurrentSession.SessionId;
			FString FullPath = FString::Printf(TEXT("/sessions/%d/participants/"), SessionId)+ FGenericPlatformHttp::UrlEncode(NetworkManager->GetUsername());
			UProVRHttpRequest::DeleteWithAuthToken(FullPath, [this, GameInstance, NetworkManager](int32 HttpResponseCode, TSharedPtr<FJsonObject> HttpResponseContent)
			{
				if (HttpResponseCode == 200)
				{
					if (UWorld* World = GameInstance->GetWorld())
					{
						UE_LOG(LogTemp, Warning, TEXT("Leave session action: 200"));
						GEngine->HandleDisconnect(World, World->GetNetDriver());
						UGameplayStatics::OpenLevel(World, "/Game/Maps/EntryMap", false, ""); // opens a new level locally
						NetworkManager->bInASession = false;
						OnLeaveSessionCompleteDelegate.Broadcast(true, EProVRLeaveSessionActionResult::ENUM_OK);

					}
				}
				else if (HttpResponseCode == 401)
				{
					UE_LOG(LogTemp, Warning, TEXT("Leave session action: on action complete: 401"));
					OnLeaveSessionCompleteDelegate.Broadcast(false, EProVRLeaveSessionActionResult::ENUM_Unauthorized);
				}
				else if (HttpResponseCode == 404)
				{
					UE_LOG(LogTemp, Warning, TEXT("Leave session action: on action complete: 404"));
					OnLeaveSessionCompleteDelegate.Broadcast(false, EProVRLeaveSessionActionResult::ENUM_UserOrSessionDoesNotExists);
				}
				else if (HttpResponseCode == 500)
				{
					UE_LOG(LogTemp, Warning, TEXT("Leave session action: on action complete: 500"));
					OnLeaveSessionCompleteDelegate.Broadcast(false, EProVRLeaveSessionActionResult::ENUM_InternalError);
				}
				else
				{
					if (HttpResponseContent->HasTypedField<EJson::String>("message"))
					{
						UE_LOG(LogTemp, Error, TEXT("%s"), *HttpResponseContent->GetStringField("message"));
					}
					UE_LOG(LogTemp, Warning, TEXT("other error leave session"));
					OnLeaveSessionCompleteDelegate.Broadcast(false, EProVRLeaveSessionActionResult::ENUM_OtherError);
				}
				OnAsyncronousActionCompleted();
			});			
		}
	}

	return EProVRActionBehavior::Asynchronous;
}
