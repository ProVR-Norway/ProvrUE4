// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRJoinSessionAction.h"
#include "ProVRGameInstance.h"
#include "Managers/ProVRNetworkManager.h"
#include "Kismet/GameplayStatics.h"

EProVRActionBehavior UProVRJoinSessionAction::PerformAction()
{
	TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject);

	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
		{

			RequestJson->SetStringField("userName", NetworkManager->GetUsername());

			UProVRHttpRequest::PostJson(SESSION_BASE_PATH + NetworkManager->GetSessionId() + "/Participants", RequestJson,
				[this](int32 HttpResponseCode, TSharedPtr<FJsonObject> HttpResponseContent)
				{
					FString Message_ = HttpResponseContent->GetStringField("message");
					if (EHttpResponseCodes::IsOk(HttpResponseCode))
					{
						if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
						{
							if (UWorld* World = GameInstance->GetWorld())
							{
								if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
								{
									FString* URLAddress_ = NetworkManager->DisplayedSessions.Find("SessionName");
									UGameplayStatics::OpenLevel(World, FName(HttpResponseContent->GetStringField("message")), false, "");

								}
							}
						}
						OnJoinSessionCompleteDelegate.Broadcast(true);
					}
					if (HttpResponseCode == 401)
					{
						UE_LOG(LogTemp, Warning, TEXT("error 401 Unauthorized.Please re - login"));
						OnJoinSessionCompleteDelegate.Broadcast(false);
					}
					if (HttpResponseCode == 404)
					{
						UE_LOG(LogTemp, Warning, TEXT("error 404 Session does not exist"));
						OnJoinSessionCompleteDelegate.Broadcast(false);
					}
					if (HttpResponseCode == 500 || HttpResponseCode == HTTP_UNEXPECTED_ERROR)
					{
						UE_LOG(LogTemp, Warning, TEXT("error 500 Internal error"));
						OnJoinSessionCompleteDelegate.Broadcast(false);
					}
					else
					{
						if (HttpResponseContent->HasTypedField<EJson::String>("message"))
						{
							UE_LOG(LogTemp, Error, TEXT("%s"), *HttpResponseContent->GetStringField("message"));
						}
						OnJoinSessionCompleteDelegate.Broadcast(false);
					}
					OnAsyncronousActionCompleted();
				});

		}
	}
	return EProVRActionBehavior::Asynchronous;
}
