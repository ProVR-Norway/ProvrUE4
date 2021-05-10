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
				OnCreateSessionCompleteDelegate.Broadcast(true, EProVRCreateSessionActionResult::ENUM_OK, JsonField);
			}
			else if (HttpResponseCode == 401)
			{
				UE_LOG(LogTemp, Warning, TEXT("error 401 Unauthorized.Please re - login"));
				OnCreateSessionCompleteDelegate.Broadcast(false, EProVRCreateSessionActionResult::ENUM_Unauthorized, -1);
			}
			else if (HttpResponseCode == 403)
			{
				UE_LOG(LogTemp, Warning, TEXT("error 403 Session with the same name already exists for the user"));
				OnCreateSessionCompleteDelegate.Broadcast(false, EProVRCreateSessionActionResult::ENUM_SessionWithSameNameExists, -1);
			}
			else if (HttpResponseCode == 404)
			{
				UE_LOG(LogTemp, Warning, TEXT("error 404 User does not exist"));
				OnCreateSessionCompleteDelegate.Broadcast(false, EProVRCreateSessionActionResult::ENUM_UserDoesNotExists, -1);
			}
			else if (HttpResponseCode == 500)
			{
				UE_LOG(LogTemp, Warning, TEXT("error 500 Internal error"));
				OnCreateSessionCompleteDelegate.Broadcast(false, EProVRCreateSessionActionResult::ENUM_InternalError, -1);
				}

			else if (HttpResponseCode == 503)
			{
				UE_LOG(LogTemp, Warning, TEXT("error 503 No servers are currently available"));
				OnCreateSessionCompleteDelegate.Broadcast(false, EProVRCreateSessionActionResult::ENUM_NoServersAvailable, -1);
				
			}
			else
			{
				if (HttpResponseContent->HasTypedField<EJson::String>("message"))
				{
					UE_LOG(LogTemp, Error, TEXT("%s"), *HttpResponseContent->GetStringField("message"));
				}
				UE_LOG(LogTemp, Warning, TEXT("other error create session"));
				OnCreateSessionCompleteDelegate.Broadcast(false, EProVRCreateSessionActionResult::ENUM_OtherError, -1);	
			}
			OnAsyncronousActionCompleted();
		});

	return EProVRActionBehavior::Asynchronous;
}


