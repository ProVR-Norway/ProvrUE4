// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRLeaveSessionAction.h"
#include "Network/ProVRHttpRequest.h"
#include "ProVRGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ProVRNetworkManager.h"

EProVRActionBehavior UProVRLeaveSessionAction::PerformAction()
{
	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UProVRNetworkManager* NetworkManager = GameInstance->GetNetworkManager())
		{
			UProVRHttpRequest::DeleteWithAuthToken(SESSION_BASE_PATH + FString::Printf(TEXT("/%d/participants/"), 
			NetworkManager->CurrentSession->SessionId) + NetworkManager->GetUsername(),
			[this, GameInstance, NetworkManager](int32 HttpResponseCode, TSharedPtr<FJsonObject> HttpResponseContent)
			{
				if (HttpResponseCode == 200)
				{
					if (UWorld* World = GameInstance->GetWorld())
					{
						OnLeaveSessionCompleteDelegate.Broadcast(true, FString(HttpResponseContent->GetStringField("message")));
						/*
						UGameplayStatics::OpenLevel(World, "/Game/Maps/EntryMap", false, "");
						NetworkManager->CurrentSession = nullptr;
						*/
					}
				}
				else if (HttpResponseCode == 401)
				{
					UE_LOG(LogTemp, Warning, TEXT("	nauthorized.Please re - login"));
					OnLeaveSessionCompleteDelegate.Broadcast(false, FString(HttpResponseContent->GetStringField("message")));
				}
				else if (HttpResponseCode == 404)
				{
					UE_LOG(LogTemp, Warning, TEXT("User or session does not exist"));
					OnLeaveSessionCompleteDelegate.Broadcast(false, FString(HttpResponseContent->GetStringField("message")));
				}
				else if (HttpResponseCode == 500)
				{
					UE_LOG(LogTemp, Warning, TEXT("Internal error"));
					OnLeaveSessionCompleteDelegate.Broadcast(false, FString(HttpResponseContent->GetStringField("message")));
				}
				else
				{
					if (HttpResponseContent->HasTypedField<EJson::String>("message"))
					{
						UE_LOG(LogTemp, Error, TEXT("%s"), *HttpResponseContent->GetStringField("message"));
					}
					OnLeaveSessionCompleteDelegate.Broadcast(false, *HttpResponseContent->GetStringField("message"));
				}
				OnAsyncronousActionCompleted();
			});			
		}
	}
	//const FString& _Path, TFunction<void(int32, TSharedPtr<FJsonObject>)> _OnResponseCompleted
	
	OnLeaveSessionCompleteDelegate.Broadcast(false, "Could Not leave session! Network manager not available!");
	
	
	return EProVRActionBehavior::Asynchronous;
}
