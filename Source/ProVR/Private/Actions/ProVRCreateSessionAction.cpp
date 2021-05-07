// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRCreateSessionAction.h"
#include "Managers/ProVRNetworkManager.h"
#include "ProVRGameInstance.h"
#include "Network/ProVRHttpRequest.h"
#include "GenericPlatform/GenericPlatform.h"
#include "..\..\Public\Actions\ProVRCreateSessionAction.h"

UProVRCreateSessionAction::UProVRCreateSessionAction()
{

}

EProVRActionBehavior UProVRCreateSessionAction::PerformAction()
{
	TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject);

	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
		{
		
		RequestJson->SetStringField("sessionName", SessionName);
		RequestJson->SetStringField("mapName", MapName);
		RequestJson->SetNumberField("maxParticipants", MaxPlayers);  //possible cast issues
		RequestJson->SetStringField("hostUsername", NetworkManager->GetUsername());
		}
	}
	UProVRHttpRequest::PostJsonWithAuthToken(SESSION_BASE_PATH, RequestJson,
		[this](int32 HttpResponseCode, TSharedPtr<FJsonObject> HttpResponseContent)
		{
			if (HttpResponseCode == 200)
			{
				int32 JsonField = HttpResponseContent->GetIntegerField("sessionId");
				FString Message_ = TEXT("Session Created Successfully");
				OnCreateSessionCompleteDelegate.Broadcast(true, Message_, JsonField);
			}
			else if (HttpResponseCode == 401)
			{
				FString WarningMessage = TEXT("error 401 Unauthorized.Please re - login");
				UE_LOG(LogTemp, Warning, TEXT("error 401 Unauthorized.Please re - login"));
				OnCreateSessionCompleteDelegate.Broadcast(false, WarningMessage, -1);
			}
			else if (HttpResponseCode == 404)
			{
				FString WarningMessage = TEXT("error 404 User does not exist");
				UE_LOG(LogTemp, Warning, TEXT("error 404 User does not exist"));
				OnCreateSessionCompleteDelegate.Broadcast(false, WarningMessage, -1);
			}
			else if (HttpResponseCode == 500)
			{
				FString WarningMessage = TEXT("error 500 Internal error");
				UE_LOG(LogTemp, Warning, TEXT("error 500 Internal error"));
				OnCreateSessionCompleteDelegate.Broadcast(false, WarningMessage, -1);
				}

			else if (HttpResponseCode == 503)
			{
				FString WarningMessage = TEXT("error 503 No servers are currently available");
				UE_LOG(LogTemp, Warning, TEXT("error 503 No servers are currently available"));
				OnCreateSessionCompleteDelegate.Broadcast(false, WarningMessage, -1);
			}
			else
			{
				if (HttpResponseContent->HasTypedField<EJson::String>("message"))
				{
					UE_LOG(LogTemp, Warning, TEXT("if (HttpResponseContent->HasTypedField<EJson::String>(message))  on create action, C++ code"));
					UE_LOG(LogTemp, Error, TEXT("%s"), *HttpResponseContent->GetStringField("message"));
				}

				OnCreateSessionCompleteDelegate.Broadcast(false, *HttpResponseContent->GetStringField("message"), -1);
			}

			OnAsyncronousActionCompleted();
		});

	return EProVRActionBehavior::Asynchronous;
}


