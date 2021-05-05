// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRCreateSessionAction.h"
#include "Managers/ProVRNetworkManager.h"
#include "ProVRGameInstance.h"
#include "ProVRHttpRequest.h"
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
		RequestJson->SetNumberField("maxParticipants", MaxPlayers);  //possible cast issues
		RequestJson->SetStringField("hostUsername", NetworkManager->GetUsername());
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
				OnCreateSessionCompleteDelegate.Broadcast(false, WarningMessage);
			}
			else if (EHttpResponseCodes::IsOk(HttpResponseCode))
			{
				FString JsonField = HttpResponseContent->GetStringField(TEXT("message"));
				OnCreateSessionCompleteDelegate.Broadcast(true, JsonField);
			}

			OnAsyncronousActionCompleted();
		});

	return EProVRActionBehavior::Asynchronous;
}
