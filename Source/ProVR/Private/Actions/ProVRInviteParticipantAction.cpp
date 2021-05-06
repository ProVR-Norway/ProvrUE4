// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRInviteParticipantAction.h"
#include "ProVRGameInstance.h"
#include "Managers/ProVRNetworkManager.h"



EProVRActionBehavior UProVRInviteParticipantAction::PerformAction()
{
	
	TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject);
	TArray<TSharedPtr<FJsonValue>> JsonContentArr;
	for (int i = 0; i < ParticipantsToInvite.Num(); i++)
	{
		TSharedPtr<FJsonObject> obj = MakeShareable(new FJsonObject);
		obj->SetStringField( "usernameOrEmail", *ParticipantsToInvite[i]);
		TSharedRef<FJsonValueObject> JsonValue_ = MakeShareable(new FJsonValueObject(obj));
		JsonContentArr.Add(JsonValue_);
	}

	//RequestJson->SetArrayField("invited");
	RequestJson->SetArrayField("invited", JsonContentArr);

	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
		{
			if (!JoinSessionAfterCreation)
			{
				for (int i = 0; i < NetworkManager->SessionList.Num(); i++)
				{
					if (NetworkManager->SessionList[i].SessionName == SessionName)
						SessionId = NetworkManager->SessionList[i].SessionId;
				}
			}
			//Request->SetURL(FString::Printf(TEXT("https://%s/sessions/%d"), *config->ServerAddress, SessionInfo->SessionIdMySQL));
			//FString URLPath = FString::Printf(TEXT("%s/%d/invited"), SESSION_BASE_PATH, SessionId);
			FString URLPath = SESSION_BASE_PATH + FString::Printf(TEXT("/%d/invited"), SessionId);
			UProVRHttpRequest::PostJson( URLPath, RequestJson,
				[this](int32 HttpResponseCode, TSharedPtr<FJsonObject> HttpResponseContent)
				{
					FString Message_ = HttpResponseContent->GetStringField("message");
					if (EHttpResponseCodes::IsOk(HttpResponseCode))
					{
						OnInviteParticipantCompleteDelegate.Broadcast(true, Message_);
					}
					if (HttpResponseCode == 401)
					{
						UE_LOG(LogTemp, Warning, TEXT("error 401 Unauthorized.Please re - login"));
						OnInviteParticipantCompleteDelegate.Broadcast(false, Message_);
					}
					if (HttpResponseCode == 404)
					{
						UE_LOG(LogTemp, Warning, TEXT("error 404 Session does not exist"));
						OnInviteParticipantCompleteDelegate.Broadcast(false, Message_);
					}
					if (HttpResponseCode == 500 || HttpResponseCode == HTTP_UNEXPECTED_ERROR)
					{
						UE_LOG(LogTemp, Warning, TEXT("error 500 Internal error"));
						OnInviteParticipantCompleteDelegate.Broadcast(false, Message_);
					}
					else
					{
						if (HttpResponseContent->HasTypedField<EJson::String>("message"))
						{
							UE_LOG(LogTemp, Error, TEXT("%s"), *HttpResponseContent->GetStringField("message"));
						}
						OnInviteParticipantCompleteDelegate.Broadcast(false, *HttpResponseContent->GetStringField("message"));
					}
					OnAsyncronousActionCompleted();
				});
		}
	}

	

	return EProVRActionBehavior::Asynchronous;

}
