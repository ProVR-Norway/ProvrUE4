// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRJoinSessionAction.h"
#include "ProVRGameInstance.h"
#include "Managers/ProVRNetworkManager.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Kismet/GameplayStatics.h"

EProVRActionBehavior UProVRJoinSessionAction::PerformAction()
{
	TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject);

	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
		{
			FString URLPathLevelToJoin;
			int32 SessionIndexInSessionList;
			RequestJson->SetStringField("username", FGenericPlatformHttp::UrlEncode(NetworkManager->GetUsername()));

			for (int i = 0; i < NetworkManager->SessionList.Num(); i++)
			{
				if (NetworkManager->SessionList[i].SessionId == SessionId)
				{
					URLPathLevelToJoin = 
					  FGenericPlatformHttp::UrlEncode(NetworkManager->SessionList[i].HostIP)
					+ FString::Printf(TEXT(":%d/Game/Maps/"), NetworkManager->SessionList[i].HostPort)
					+ FGenericPlatformHttp::UrlEncode(NetworkManager->SessionList[i].MapName);
					SessionIndexInSessionList = i;
				}
			}

			
			FString URLPathGateway = SESSION_BASE_PATH + FString::Printf(TEXT("/%d/participants"), SessionId);
			UProVRHttpRequest::PostJsonWithAuthToken(URLPathGateway, RequestJson,

				[this, NetworkManager, GameInstance, URLPathLevelToJoin, SessionIndexInSessionList](int32 HttpResponseCode, TSharedPtr<FJsonObject> HttpResponseContent)
				{
					FString Message_ = HttpResponseContent->GetStringField("message");
					if (HttpResponseCode == 200)
					{

						if(UWorld* World = GameInstance->GetWorld())
						{
							FString* URLAddress_ = NetworkManager->DisplayedSessions.Find("SessionName");
							UGameplayStatics::OpenLevel(World, FName(URLPathLevelToJoin), false, "");
							UE_LOG(LogTemp, Warning, TEXT("%d"), *URLPathLevelToJoin);
							//UGameplayStatics::OpenLevel(World, "34.90.23.60:7777/Game/Maps/TestMap", false, "");
							NetworkManager->CurrentSession.HostIP			= NetworkManager->SessionList[SessionIndexInSessionList].HostIP;
							NetworkManager->CurrentSession.HostPort			= NetworkManager->SessionList[SessionIndexInSessionList].HostPort;
							NetworkManager->CurrentSession.HostUsername		= NetworkManager->SessionList[SessionIndexInSessionList].HostUsername;
							NetworkManager->CurrentSession.MapName			= NetworkManager->SessionList[SessionIndexInSessionList].MapName;
							NetworkManager->CurrentSession.MaxParticipants  = NetworkManager->SessionList[SessionIndexInSessionList].MaxParticipants;
							NetworkManager->CurrentSession.SessionId		= NetworkManager->SessionList[SessionIndexInSessionList].SessionId;
							NetworkManager->CurrentSession.SessionName		= NetworkManager->SessionList[SessionIndexInSessionList].SessionName;
							NetworkManager->bInASession = true;
							NetworkManager->SessionList.Empty();
							OnJoinSessionCompleteDelegate.Broadcast(true, EProVRJoinSessionActionResult::ENUM_OK);
						}	
					}
					else if (HttpResponseCode == 401)
					{
						UE_LOG(LogTemp, Warning, TEXT("error 401 Unauthorized.Please re - login"));
						OnJoinSessionCompleteDelegate.Broadcast(false, EProVRJoinSessionActionResult::ENUM_Unauthorized);
					}
					else if (HttpResponseCode == 404)
					{
						UE_LOG(LogTemp, Warning, TEXT("error 404 Session does not exist"));
						OnJoinSessionCompleteDelegate.Broadcast(false, EProVRJoinSessionActionResult::ENUM_UserOrSessionDoesNotExists);
					}
					else if (HttpResponseCode == 500 || HttpResponseCode == HTTP_UNEXPECTED_ERROR)
					{
						UE_LOG(LogTemp, Warning, TEXT("error 500 Internal error"));
						OnJoinSessionCompleteDelegate.Broadcast(false, EProVRJoinSessionActionResult::ENUM_InternalError);
					}
					else
					{
						if (HttpResponseContent->HasTypedField<EJson::String>("message"))
						{
							UE_LOG(LogTemp, Error, TEXT("%s"), *HttpResponseContent->GetStringField("message"));
						}
						UE_LOG(LogTemp, Warning, TEXT("other error join session"));
						OnJoinSessionCompleteDelegate.Broadcast(false, EProVRJoinSessionActionResult::ENUM_OtherError);
					}
					OnAsyncronousActionCompleted();
				});

		}
	}
	return EProVRActionBehavior::Asynchronous;
}
