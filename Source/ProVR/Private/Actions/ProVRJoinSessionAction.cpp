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
			RequestJson->SetStringField("username", NetworkManager->GetUsername());
			FString URLPathLevelToJoin;
			if (!JoinSessionAfterCreation)
			{
				for (int i = 0; i < NetworkManager->SessionList.Num(); i++)
				{
					if (NetworkManager->SessionList[i].SessionName == SessionName)
					{
						SessionId = NetworkManager->SessionList[i].SessionId;
						URLPathLevelToJoin = 
						  FGenericPlatformHttp::UrlEncode(NetworkManager->SessionList[i].HostIP)
						+ FString::Printf(TEXT(":%d/Game/Maps/"), NetworkManager->SessionList[i].HostPort)
						+ FGenericPlatformHttp::UrlEncode(NetworkManager->SessionList[i].MapName);
					}
				}
			}
			
			FString URLPath_ = SESSION_BASE_PATH + FString::Printf(TEXT("/%d/participants"), SessionId);
			UProVRHttpRequest::PostJsonWithAuthToken(URLPath_, RequestJson,

				[this, NetworkManager, GameInstance, URLPathLevelToJoin](int32 HttpResponseCode, TSharedPtr<FJsonObject> HttpResponseContent)
				{
					FString Message_ = HttpResponseContent->GetStringField("message");
					if (HttpResponseCode == 200)
					{

						if(UWorld* World = GameInstance->GetWorld())
						{
							FString* URLAddress_ = NetworkManager->DisplayedSessions.Find("SessionName");
							UGameplayStatics::OpenLevel(World, FName(URLPathLevelToJoin), false, "");
							//UGameplayStatics::OpenLevel(World, false, "");
							OnJoinSessionCompleteDelegate.Broadcast(true, "Joined successful!!");
						}
	
					}
					else if (HttpResponseCode == 401)
					{
						UE_LOG(LogTemp, Warning, TEXT("error 401 Unauthorized.Please re - login"));
						OnJoinSessionCompleteDelegate.Broadcast(false, TEXT("error 401 Unauthorized.Please re - login"));
					}
					else if (HttpResponseCode == 404)
					{
						UE_LOG(LogTemp, Warning, TEXT("error 404 Session does not exist"));
						OnJoinSessionCompleteDelegate.Broadcast(false, TEXT("error 404 Session does not exist"));
					}
					else if (HttpResponseCode == 500 || HttpResponseCode == HTTP_UNEXPECTED_ERROR)
					{
						UE_LOG(LogTemp, Warning, TEXT("error 500 Internal error"));
						OnJoinSessionCompleteDelegate.Broadcast(false, TEXT("error 500 Internal error"));
					}
					else
					{
						if (HttpResponseContent->HasTypedField<EJson::String>("message"))
						{
							UE_LOG(LogTemp, Error, TEXT("%s"), *HttpResponseContent->GetStringField("message"));
						}
						OnJoinSessionCompleteDelegate.Broadcast(false, *HttpResponseContent->GetStringField("message"));
					}
					OnAsyncronousActionCompleted();
				});

		}
	}
	return EProVRActionBehavior::Asynchronous;
}
