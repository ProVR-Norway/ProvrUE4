// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRFindSessionAction.h"
#include "Network/ProVRHttpRequest.h"
#include "ProVRGameInstance.h"
#include "Managers/ProVRNetworkManager.h"


EProVRActionBehavior UProVRFindSessionAction::PerformAction()
{
	TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject);

	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
		{
			UProVRHttpRequest::Get(SESSION_BASE_PATH + "?username=" + NetworkManager->GetUsername(),  // query to API
				[this](int32 HttpResponseCode, TSharedPtr<FJsonObject> HttpResponseContent)
				{
					if (EHttpResponseCodes::IsOk(HttpResponseCode))
					{

						if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance()) //Game instance is not in lambda scoop line 19
						{
							if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
							{
								NetworkManager->SessionList.Empty(); // deletes the old session list
								TArray<TSharedPtr<FJsonValue>> JsonList = HttpResponseContent->GetArrayField("sessionName");
								for (int32 index = 0; index < JsonList.Num(); index++)
								{
									TSharedPtr<FJsonObject> CurrentSelectedSession = JsonList[index]->AsObject();
									FProVRSessionsOverview SessionToBeQueued;
									SessionToBeQueued.SessionId = CurrentSelectedSession->GetIntegerField("sessionId");
									SessionToBeQueued.SessionName = CurrentSelectedSession->GetStringField("sessionName");
									SessionToBeQueued.MapName = CurrentSelectedSession->GetStringField("mapName");
									SessionToBeQueued.MaxParticipants = CurrentSelectedSession->GetIntegerField("maxParticipants");
									SessionToBeQueued.HostUsername = CurrentSelectedSession->GetStringField("hostUsername");
									SessionToBeQueued.HostIP = CurrentSelectedSession->GetStringField("hostIP");
									SessionToBeQueued.HostPort = CurrentSelectedSession->GetIntegerField("hostPort");
									NetworkManager->SessionList.Add(SessionToBeQueued); // adds the new session list
									OnFindSessionCompleteDelegate.Broadcast(true, "Session Created!");
								}
							}
						}
						if (HttpResponseCode == 401)
						{
							FString WarningMessage = TEXT("error 401 Unauthorized.Please re - login");
							UE_LOG(LogTemp, Warning, TEXT("error 401 Unauthorized.Please re - login"));
							OnFindSessionCompleteDelegate.Broadcast(false, WarningMessage);
						}
						if (HttpResponseCode == 404)
						{
							FString WarningMessage = TEXT("error 404 Session does not exist");
							UE_LOG(LogTemp, Warning, TEXT("error 404 Session does not exist"));
							OnFindSessionCompleteDelegate.Broadcast(false, WarningMessage);
						}
						if (HttpResponseCode == 500 || HttpResponseCode == HTTP_UNEXPECTED_ERROR)
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
					}
					OnAsyncronousActionCompleted();
				});
		}
	}

	return EProVRActionBehavior::Asynchronous;
}
