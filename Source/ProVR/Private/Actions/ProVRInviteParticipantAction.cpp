// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRInviteParticipantAction.h"



EProVRActionBehavior UProVRInviteParticipantAction::PerformAction()
{
	/*
	TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject);

	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
		{

			//RequestJson->SetArrayField();
		}
	}

	else
	{
		OnAsyncronousActionCompleted(); // Do something else here!!!!
	}

	UProVRHttpRequest::PostJson("RandomAddress", RequestJson,
		[this](int32 HttpResponseCode, TSharedPtr<FJsonObject> HttpResponseContent)
		{
			if (!EHttpResponseCodes::IsOk(HttpResponseCode))
			{
				FString WarningMessage = TEXT("Somewent wrong!");
				UE_LOG(LogTemp, Warning, TEXT("Error Creating Session: UProVRSessionInterface::OnCreateSessionComplete"));
				OnActionComplete.Broadcast(false, WarningMessage);
			}
			else if (EHttpResponseCodes::IsOk(HttpResponseCode))
			{
				FString JsonField = HttpResponseContent->GetStringField(TEXT("message"));
				OnActionComplete.Broadcast(true, JsonField);
			}

			OnAsyncronousActionCompleted();
		});

	return EProVRActionBehavior::Asynchronous
	*/
	return EProVRActionBehavior::Synchronous;
}
