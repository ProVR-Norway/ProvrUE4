// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "Managers/ProVRNetworkManager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

#include "ProVRGameInstance.h"
#include "Kismet/GameplayStatics.h"
void UProVRNetworkManager::OpenLeverTest()
{
	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UWorld* World = GameInstance->GetWorld())
		{
			UGameplayStatics::OpenLevel(World, "34.90.23.60:7777/Game/Maps/TestMap", false, "");
		}
	}
}


void UProVRNetworkManager::PushNewHttpRequest(UProVRHttpRequest* NewHttpRequest)
{
	ActiveHttpRequests.AddUnique(NewHttpRequest);
}

void UProVRNetworkManager::RemoveHttpRequest(UProVRHttpRequest* HttpRequest)
{
	ActiveHttpRequests.Remove(HttpRequest);
}

//EmailAdress -> Username | David 15.04
void UProVRNetworkManager::PerformLoginRequest(const FString& Username, const FString& Password, TFunction<void(int32)> OnCompleted) //EmailAdress -> Username
{
	LastUsername = Username; //LastEmailAdress -> LastUsername | David 15.04
	LastPassword = Password;

	if (OngoingTryRenewingAuthTokenRequest.IsValid())
	{
		bWasOngoingTryRenewingAuthTokenRequestCanceled = true;
		bTryRenewingAuthTokenRequestIsOngoing = false;
		OngoingTryRenewingAuthTokenRequest->CancelRequest();
	}

	TryRenewingAuthToken(OnCompleted);
}

void UProVRNetworkManager::TryRenewingAuthToken(TFunction<void(int32)> OnCompleted)
{
	OngoingTryRenewingAuthTokenRequestSubscribers.Add(OnCompleted);

	if (bTryRenewingAuthTokenRequestIsOngoing) return;
	bTryRenewingAuthTokenRequestIsOngoing = true;

	OngoingTryRenewingAuthTokenRequest = FHttpModule::Get().CreateRequest();

	OngoingTryRenewingAuthTokenRequest->OnProcessRequestComplete().BindUObject(this, &UProVRNetworkManager::OnRenewingAuthTokenRequestCompleted);

	OngoingTryRenewingAuthTokenRequest->SetURL(BACKEND_BASE_URL + AUTH_SERVICE_LOGIN_REQUEST_PATH);
	OngoingTryRenewingAuthTokenRequest->SetVerb("POST");
	OngoingTryRenewingAuthTokenRequest->SetHeader("Content-Type", "application/json");
	//emailAdress -> username | 15.04 David
	TSharedPtr<FJsonObject> LoginRequestContent = MakeShareable(new FJsonObject);
	LoginRequestContent->SetStringField("username", LastUsername); //LastEmailAdress -> LastUsername | 15.04 David
	LoginRequestContent->SetStringField("password", LastPassword);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(LoginRequestContent.ToSharedRef(), Writer);
	OngoingTryRenewingAuthTokenRequest->SetContentAsString(OutputString);

	if (!OngoingTryRenewingAuthTokenRequest->ProcessRequest())
	{
		CallSubscribersAfterTryRenewingAuthTokenResponse(HTTP_UNEXPECTED_ERROR);
	}
}

void UProVRNetworkManager::OnRenewingAuthTokenRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasOngoingTryRenewingAuthTokenRequestCanceled)
	{
		//If this callback is called due to the cancellation (meaning that there is another request upcoming, do not call subscribers.
		bWasOngoingTryRenewingAuthTokenRequestCanceled = false;
		return;
	}

	bTryRenewingAuthTokenRequestIsOngoing = false;

	if (!bWasSuccessful || !Response.IsValid())
	{
		CallSubscribersAfterTryRenewingAuthTokenResponse(HTTP_UNEXPECTED_ERROR);
	}
	else
	{
		int32 ResponseCode = Response->GetResponseCode();
		if (!EHttpResponseCodes::IsOk(ResponseCode))
		{
			CallSubscribersAfterTryRenewingAuthTokenResponse(ResponseCode);
		}
		else
		{
			TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
			if (!FJsonSerializer::Deserialize(JsonReader, JsonObject)
				|| !JsonObject.IsValid()
				|| !JsonObject->HasTypedField<EJson::String>("token"))
			{
				CallSubscribersAfterTryRenewingAuthTokenResponse(HTTP_UNEXPECTED_ERROR);
			}
			else
			{
				CurrentAuthToken = JsonObject->GetStringField("token");

				CallSubscribersAfterTryRenewingAuthTokenResponse(ResponseCode);
			}
		}
	}

	OngoingTryRenewingAuthTokenRequest.Reset();
}

void UProVRNetworkManager::CallSubscribersAfterTryRenewingAuthTokenResponse(int32 HttpResponseCode)
{
	for (auto& Subscriber : OngoingTryRenewingAuthTokenRequestSubscribers)
	{
		if (Subscriber)
		{
			Subscriber(HttpResponseCode);
		}
	}

	OngoingTryRenewingAuthTokenRequestSubscribers.Empty(); //Clear the array
}

FString UProVRNetworkManager::GetUsername()
{
	return LastUsername;
}