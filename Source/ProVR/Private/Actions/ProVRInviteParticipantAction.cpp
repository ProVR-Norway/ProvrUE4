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
			UProVRHttpRequest::PostJsonWithAuthToken(URLPath, RequestJson,
				[this](int32 HttpResponseCode, TSharedPtr<FJsonObject> HttpResponseContent)
				{
					if (HttpResponseCode == 200)
					{
						UE_LOG(LogTemp, Warning, TEXT("Invite participants action: on action complete: 200"));
						OnInviteParticipantCompleteDelegate.Broadcast(true, EProVRInviteParticipantActionResult::ENUM_OK);
					}
					else if (HttpResponseCode == 401)
					{
						UE_LOG(LogTemp, Warning, TEXT("Invite participants action: on action complete: 401"));
						OnInviteParticipantCompleteDelegate.Broadcast(false, EProVRInviteParticipantActionResult::ENUM_Unauthorized);
					}
					else if (HttpResponseCode == 404)
					{
						UE_LOG(LogTemp, Warning, TEXT("Invite participants action: on action complete: 404"));
						OnInviteParticipantCompleteDelegate.Broadcast(false, EProVRInviteParticipantActionResult::ENUM_UserDoesNotExists);
					}
					else if (HttpResponseCode == 500 || HttpResponseCode == HTTP_UNEXPECTED_ERROR)
					{
						UE_LOG(LogTemp, Warning, TEXT("Invite participants action: on action complete: 500"));
						OnInviteParticipantCompleteDelegate.Broadcast(false, EProVRInviteParticipantActionResult::ENUM_InternalError);
					}
					else
					{
						if (HttpResponseContent->HasTypedField<EJson::String>("message"))
						{
							UE_LOG(LogTemp, Error, TEXT("%s"), *HttpResponseContent->GetStringField("message"));
						}
						UE_LOG(LogTemp, Warning, TEXT("other error invite session"));
						OnInviteParticipantCompleteDelegate.Broadcast(false, EProVRInviteParticipantActionResult::ENUM_OtherError);
					}
					OnAsyncronousActionCompleted();
				});
		}
	}

	return EProVRActionBehavior::Asynchronous;

}
