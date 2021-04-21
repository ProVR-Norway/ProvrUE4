// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "Network/ProVRHttpRequest.h"
#include "ProVRGameInstance.h"
#include "Managers/ProVRNetworkManager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

void UProVRHttpRequest::Get(const FString& _Path, TFunction<void(int32, TSharedPtr<FJsonObject>)> _OnResponseCompleted)
{
	if (UProVRHttpRequest* CreatedRequest = CreateInternalRequest(_Path, _OnResponseCompleted))
	{
		CreatedRequest->InternalHttpRequest->SetVerb("GET");
		CreatedRequest->InternalHttpRequest->SetHeader("Authorization", "Basic b17ffe64aeb9de8c33081633bf403e57e53d4f871f4eaa4a219d2f3325d33a201cfd930673606027cb7449e21f9805b49404c7b01c0b579c23dfa8d3a5fb13ca");
		CreatedRequest->RequestType = EHttpRequestType::ENUM_Get;
		CreatedRequest->ProcessInternalRequest();
	}
	else
	{
		_OnResponseCompleted(HTTP_UNEXPECTED_ERROR, ErrorMessageJson(TEXT("Failed to create a request.")));
	}
}

void UProVRHttpRequest::Delete(const FString& _Path, TFunction<void(int32, TSharedPtr<FJsonObject>)> _OnResponseCompleted)
{
	if (UProVRHttpRequest* CreatedRequest = CreateInternalRequest(_Path, _OnResponseCompleted))
	{
		CreatedRequest->InternalHttpRequest->SetVerb("DELETE");
		CreatedRequest->RequestType = EHttpRequestType::ENUM_Delete;
		CreatedRequest->ProcessInternalRequest();
	}
	else
	{
		_OnResponseCompleted(HTTP_UNEXPECTED_ERROR, ErrorMessageJson(TEXT("Failed to create a request.")));
	}
}

void UProVRHttpRequest::PostJson(const FString& _Path, TSharedPtr<FJsonObject> _Content, TFunction<void(int32, TSharedPtr<FJsonObject>)> _OnResponseCompleted)
{
	if (!_Content.IsValid())
	{
		_OnResponseCompleted(HTTP_UNEXPECTED_ERROR, ErrorMessageJson(TEXT("Content is invalid.")));
		return;
	}

	if (UProVRHttpRequest* CreatedRequest = CreateInternalRequest(_Path, _OnResponseCompleted))
	{
		CreatedRequest->InternalHttpRequest->SetVerb("POST");
		CreatedRequest->InternalHttpRequest->SetHeader("Content-Type", "application/json");
		CreatedRequest->RequestType = EHttpRequestType::ENUM_Post;

		FString OutputString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(_Content.ToSharedRef(), Writer);

		
		const FTCHARToUTF8 Converter(*OutputString, OutputString.Len());
		CreatedRequest->RequestContent.Append(reinterpret_cast<const uint8*>(Converter.Get()), Converter.Length());
		CreatedRequest->InternalHttpRequest->SetContent(CreatedRequest->RequestContent);
		
		//CreatedRequest->InternalHttpRequest->SetContentAsString(OutputString);
		CreatedRequest->ProcessInternalRequest();
	}
	else
	{
		_OnResponseCompleted(HTTP_UNEXPECTED_ERROR, ErrorMessageJson(TEXT("Failed to create a request.")));
	}
}

void UProVRHttpRequest::PutJson(const FString& _Path, TSharedPtr<FJsonObject> _Content, TFunction<void(int32, TSharedPtr<FJsonObject>)> _OnResponseCompleted)
{
	if (!_Content.IsValid())
	{
		_OnResponseCompleted(HTTP_UNEXPECTED_ERROR, ErrorMessageJson(TEXT("Content is invalid.")));
		return;
	}

	if (UProVRHttpRequest* CreatedRequest = CreateInternalRequest(_Path, _OnResponseCompleted))
	{
		CreatedRequest->InternalHttpRequest->SetVerb("PUT");
		CreatedRequest->RequestType = EHttpRequestType::ENUM_Put;

		FString OutputString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(_Content.ToSharedRef(), Writer);

		const FTCHARToUTF8 Converter(*OutputString, OutputString.Len());
		CreatedRequest->RequestContent.Append(reinterpret_cast<const uint8*>(Converter.Get()), Converter.Length());
		CreatedRequest->InternalHttpRequest->SetContent(CreatedRequest->RequestContent);

		CreatedRequest->ProcessInternalRequest();
	}
	else
	{
		_OnResponseCompleted(HTTP_UNEXPECTED_ERROR, ErrorMessageJson(TEXT("Failed to create a request.")));
	}
}

void UProVRHttpRequest::PutFile(const FString& _Path, const TArray<uint8>& _Content, TFunction<void(int32, TSharedPtr<FJsonObject>)> _OnResponseCompleted)
{
	if (_Content.Num() == 0)
	{
		_OnResponseCompleted(HTTP_UNEXPECTED_ERROR, ErrorMessageJson(TEXT("Content is invalid.")));
		return;
	}

	if (UProVRHttpRequest* CreatedRequest = CreateInternalRequest(_Path, _OnResponseCompleted))
	{
		CreatedRequest->RequestContent = _Content;

		CreatedRequest->InternalHttpRequest->SetVerb("PUT");
		CreatedRequest->RequestType = EHttpRequestType::ENUM_Put;
		CreatedRequest->InternalHttpRequest->SetContent(_Content);

		CreatedRequest->ProcessInternalRequest();
	}
	else
	{
		_OnResponseCompleted(HTTP_UNEXPECTED_ERROR, ErrorMessageJson(TEXT("Failed to create a request.")));
	}
}

UProVRHttpRequest* UProVRHttpRequest::CreateInternalRequest(const FString& _Path, TFunction<void(int32, TSharedPtr<FJsonObject>)> _OnResponseCompleted)
{
	if (UProVRNetworkManager* NetworkManager = UProVRGameInstance::GetNetworkManager())
	{
		UProVRHttpRequest* NewHttpRequest = NewObject<UProVRHttpRequest>();
		
		NewHttpRequest->OnResponseCompleted = _OnResponseCompleted;

		NewHttpRequest->InternalHttpRequest = FHttpModule::Get().CreateRequest();

		NewHttpRequest->InternalHttpRequest->OnProcessRequestComplete().BindUObject(NewHttpRequest, &UProVRHttpRequest::OnInternalRequestCompleted);

		NewHttpRequest->RequestFinalUrl = BACKEND_BASE_URL + _Path;
		NewHttpRequest->InternalHttpRequest->SetURL(NewHttpRequest->RequestFinalUrl);

		NewHttpRequest->InternalHttpRequest->SetHeader("Authentication", NetworkManager->GetCurrentAuthToken());

		NetworkManager->PushNewHttpRequest(NewHttpRequest);

		return NewHttpRequest;
	}
	return nullptr;
}

void UProVRHttpRequest::ProcessInternalRequest()
{
	if (!InternalHttpRequest->ProcessRequest()) //Fails to initialize the request
	{
		OnResponseCompleted(HTTP_UNEXPECTED_ERROR, ErrorMessageJson(TEXT("Failed to initialize the request.")));

		if (UProVRNetworkManager* NetworkManager = UProVRGameInstance::GetNetworkManager())
		{
			NetworkManager->RemoveHttpRequest(this);
		}
	}
}

void UProVRHttpRequest::OnInternalRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (UProVRNetworkManager* NetworkManager = UProVRGameInstance::GetNetworkManager())
	{
		if (!bWasSuccessful || !Response.IsValid())
		{
			OnResponseCompleted(HTTP_UNEXPECTED_ERROR, ErrorMessageJson(TEXT("Request was not successful.")));
		}
		else
		{
			int32 ResponseCode = Response->GetResponseCode();

			if (ResponseCode >= 500) //Retry in case an internal server error
			{
				if (++InternalErrorRetriedCount >= INTERNAL_ERROR_RETRY_TIMES)
				{
					//Exhausted
					OnResponseCompleted(ResponseCode, ErrorMessageJson("Exhausted after retrying " + FString::FromInt(INTERNAL_ERROR_RETRY_TIMES) + " times: " + Response->GetContentAsString()));
				}
				else
				{
					RetryRequest();
					return; //Return here, so the request will not be removed from NetworkManager at the end.
				}
			}
			else if (ResponseCode == 401) //Retry in case the token is expired
			{
				NetworkManager->TryRenewingAuthToken([this](int32 RenewTokenResponseCode)
					{
						if (EHttpResponseCodes::IsOk(RenewTokenResponseCode))
						{
							RetryRequest();
						}
						else
						{
							OnResponseCompleted(401, ErrorMessageJson("Unauthorized."));

							if (UProVRNetworkManager* NetworkManager = UProVRGameInstance::GetNetworkManager())
							{
								NetworkManager->RemoveHttpRequest(this); //Now remove the request from NetworkManager.
							}
						}
					});
				return; //Return here, so the request will not be removed from NetworkManager at the end.
			}
			else
			{
				TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());
				TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
				if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
				{
					OnResponseCompleted(ResponseCode, ErrorMessageJson("Failed to deserialize the response: " + Response->GetContentAsString()));
				}
				else
				{
					OnResponseCompleted(ResponseCode, JsonObject);
				}
			}
		}

		NetworkManager->RemoveHttpRequest(this); //Remove the request from NetworkManager
	}
	else
	{
		OnResponseCompleted(HTTP_UNEXPECTED_ERROR, ErrorMessageJson(TEXT("Failed to get reference to NetworkManager.")));
	}
}

void UProVRHttpRequest::RetryRequest()
{
	if (UProVRNetworkManager* NetworkManager = UProVRGameInstance::GetNetworkManager())
	{
		InternalHttpRequest = FHttpModule::Get().CreateRequest();

		InternalHttpRequest->OnProcessRequestComplete().BindUObject(this, &UProVRHttpRequest::OnInternalRequestCompleted);

		InternalHttpRequest->SetURL(RequestFinalUrl);

		InternalHttpRequest->SetHeader("Authentication", NetworkManager->GetCurrentAuthToken());

		switch (RequestType)
		{
		case EHttpRequestType::ENUM_Get:
			InternalHttpRequest->SetVerb("GET");
			break;
		case EHttpRequestType::ENUM_Delete:
			InternalHttpRequest->SetVerb("DELETE");
			break;
		case EHttpRequestType::ENUM_Post:
			InternalHttpRequest->SetVerb("POST");
			InternalHttpRequest->SetContent(RequestContent);
			break;
		case EHttpRequestType::ENUM_Put:
			InternalHttpRequest->SetVerb("PUT");
			InternalHttpRequest->SetContent(RequestContent);
			break;
		}

		InternalHttpRequest->ProcessRequest();
	}
}

TSharedPtr<FJsonObject> UProVRHttpRequest::ErrorMessageJson(const FString& Message)
{
	TSharedPtr<FJsonObject> ErrorJson = MakeShareable(new FJsonObject);
	ErrorJson->SetStringField("message", Message);
	return ErrorJson;
}