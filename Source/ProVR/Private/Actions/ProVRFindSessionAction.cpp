// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRFindSessionAction.h"
#include "ProVRHttpRequest.h"


EProVRActionBehavior UProVRFindSessionAction::PerformAction()
{
	TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject);

	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
		{
			UProVRHttpRequest::Get("RandomAddress" + NetworkManager->GetUsername(), RequestJson,
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
						TSharedPtr<FJsonObject> JsonObject;
						TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(HttpResponseContent->GetContentAsString());
						//DisplayedSessions.Add(FString, FString);
					}

					OnAsyncronousActionCompleted();
				});

				}
	}

	return EProVRActionBehavior::Asynchronous;
}
