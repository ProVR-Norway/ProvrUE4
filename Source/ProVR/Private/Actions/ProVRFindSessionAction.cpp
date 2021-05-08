// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRFindSessionAction.h"
#include "Network/ProVRHttpRequest.h"
#include "ProVRGameInstance.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Managers/ProVRNetworkManager.h"


EProVRActionBehavior UProVRFindSessionAction::PerformAction()
{
	TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject);

	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
		{
			UProVRHttpRequest::GetWithAuthToken(SESSION_BASE_PATH + "?username=" + FGenericPlatformHttp::UrlEncode(NetworkManager->GetUsername()),  // query to API
				[this, GameInstance, NetworkManager](int32 HttpResponseCode, TSharedPtr<FJsonObject> HttpResponseContent)
				{
					if (HttpResponseCode == 200)
					{
						NetworkManager->SessionList.Empty(); // deletes the old session list
						TArray<TSharedPtr<FJsonValue>> JsonList = HttpResponseContent->GetArrayField("sessions");
						for (int32 index = 0; index < JsonList.Num(); index++)
						{
							TSharedPtr<FJsonObject> CurrentSelectedSession = JsonList[index]->AsObject();
							FProVRSessionsOverview SessionToBeQueued;
							SessionToBeQueued.SessionId			= CurrentSelectedSession->GetIntegerField("sessionId");
							SessionToBeQueued.SessionName		= CurrentSelectedSession->GetStringField("sessionName");
							SessionToBeQueued.MapName			= CurrentSelectedSession->GetStringField("mapName");
							SessionToBeQueued.MaxParticipants	= CurrentSelectedSession->GetIntegerField("maxParticipants");
							SessionToBeQueued.HostUsername		= CurrentSelectedSession->GetStringField("hostUserId");
							SessionToBeQueued.HostIP			= CurrentSelectedSession->GetStringField("hostIP");
							SessionToBeQueued.HostPort			= CurrentSelectedSession->GetIntegerField("hostPort");
							NetworkManager->SessionList.Add(SessionToBeQueued); // adds the new session to the  list
						}
						OnFindSessionCompleteDelegate.Broadcast(true, "Session Created!");
					}
					else if (HttpResponseCode == 401)
					{
						FString WarningMessage = TEXT("error 401 Unauthorized.Please re - login");
						UE_LOG(LogTemp, Warning, TEXT("error 401 Unauthorized.Please re - login"));
						OnFindSessionCompleteDelegate.Broadcast(false, WarningMessage);
					}
					else if (HttpResponseCode == 404)
					{
						FString WarningMessage = TEXT("error 404 Session does not exist");
						UE_LOG(LogTemp, Warning, TEXT("error 404 Session does not exist"));
						OnFindSessionCompleteDelegate.Broadcast(false, WarningMessage);
					}
					else if (HttpResponseCode == 500 || HttpResponseCode == HTTP_UNEXPECTED_ERROR)
					{
						FString WarningMessage = TEXT("error 500 Internal error");
						UE_LOG(LogTemp, Warning, TEXT("error 500 Internal error"));
						OnFindSessionCompleteDelegate.Broadcast(false, WarningMessage);
					}
					else
					{
						if (HttpResponseContent->HasTypedField<EJson::String>("message"))
						{
							UE_LOG(LogTemp, Error, TEXT("%s"), *HttpResponseContent->GetStringField("message"));
						}
						OnFindSessionCompleteDelegate.Broadcast(false, *HttpResponseContent->GetStringField("message"));
					}


					OnAsyncronousActionCompleted();
				});
		}
		
	}

	return EProVRActionBehavior::Asynchronous;
}
