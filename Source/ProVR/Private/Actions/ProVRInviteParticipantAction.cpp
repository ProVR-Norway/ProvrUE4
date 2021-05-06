// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRInviteParticipantAction.h"
#include "ProVRGameInstance.h"
#include "Managers/ProVRNetworkManager.h"



EProVRActionBehavior UProVRInviteParticipantAction::PerformAction()
{
	
	TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject);
	TArray<TSharedPtr<FJsonValue>> JsonContent;
	for (int i = 0; i < ParticipantsToInvite.Num(); i++)
	{
		JsonContent.Add(ParticipantsToInvite[i]);
	}
	RequestJson->SetArrayField(JsonContent);

	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
		{
			int32 SessionID = NetworkManager->GetSessionId();
			UProVRHttpRequest::PostJson(SESSION_BASE_PATH + SessionID + "invited" , RequestJson,
				[this](int32 HttpResponseCode, TSharedPtr<FJsonObject> HttpResponseContent)
				{
					FString Message_ = HttpResponseContent->GetStringField();
					if (EHttpResponseCodes::IsOk(HttpResponseCode))
					{
						OnInviteParticipantComplete.Broadcast(true, Message_);
					}
					if (HttpResponseCode == 401)
					{
						UE_LOG(LogTemp, Warning, TEXT("error 401 Unauthorized.Please re - login"));
					OnInviteParticipantComplete.Broadcast(false, Message_);
					}
					if (HttpResponseCode == 404)
					{
						UE_LOG(LogTemp, Warning, TEXT("error 404 Session does not exist"));
						OnInviteParticipantComplete.Broadcast(false, Message_);
					}
					if (HttpResponseCode == 500 || HttpResponseCode == HTTP_UNEXPECTED_ERROR)
					{
						UE_LOG(LogTemp, Warning, TEXT("error 500 Internal error"));
						OnInviteParticipantComplete.Broadcast(false, Message_);
					}
					else
					{
						if (HttpResponseContent->HasTypedField<EJson::String>("message"))
						{
							UE_LOG(LogTemp, Error, TEXT("%s"), *HttpResponseContent->GetStringField("message"));
						}
						OnInviteParticipantComplete.Broadcast(false, *HttpResponseContent->GetStringField("message"));
					}
					OnAsyncronousActionCompleted();
				});
		}
	}

	

	return EProVRActionBehavior::Asynchronous;

}
