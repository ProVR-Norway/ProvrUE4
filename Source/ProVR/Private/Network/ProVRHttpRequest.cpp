// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "Network/ProVRHttpRequest.h"
#include "ProVRGameInstance.h"
#include "Managers/ProVRNetworkManager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

void UProVRHttpRequest::Get(const FString& _Path, TFunction<void(bool, int32, TSharedPtr<FJsonObject>)> _OnResponseCompleted)
{
	if (UProVRHttpRequest* CreatedRequest = CreateInternalRequest(_Path, _OnResponseCompleted))
	{
		CreatedRequest->InternalHttpRequest->SetVerb("GET");
		CreatedRequest->RequestType = EHttpRequestType::ENUM_Get;
	}
	else
	{
		_OnResponseCompleted(false, -1, ErrorMessageJson(TEXT("Failed to create a request.")));
	}
}

void UProVRHttpRequest::Delete(const FString& _Path, TFunction<void(bool, int32, TSharedPtr<FJsonObject>)> _OnResponseCompleted)
{
	if (UProVRHttpRequest* CreatedRequest = CreateInternalRequest(_Path, _OnResponseCompleted))
	{
		CreatedRequest->InternalHttpRequest->SetVerb("DELETE");
		CreatedRequest->RequestType = EHttpRequestType::ENUM_Delete;
		CreatedRequest->ProcessInternalRequest();
	}
	else
	{
		_OnResponseCompleted(false, -1, ErrorMessageJson(TEXT("Failed to create a request.")));
	}
}

void UProVRHttpRequest::PostJson(const FString& _Path, TSharedPtr<FJsonObject> _Content, TFunction<void(bool, int32, TSharedPtr<FJsonObject>)> _OnResponseCompleted)
{
	if (!_Content.IsValid())
	{
		_OnResponseCompleted(false, -1, ErrorMessageJson(TEXT("Content is invalid.")));
		return;
	}

	if (UProVRHttpRequest* CreatedRequest = CreateInternalRequest(_Path, _OnResponseCompleted))
	{
		CreatedRequest->InternalHttpRequest->SetVerb("POST");
		CreatedRequest->RequestType = EHttpRequestType::ENUM_Post;

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
		_OnResponseCompleted(false, -1, ErrorMessageJson(TEXT("Failed to create a request.")));
	}
}

void UProVRHttpRequest::PutJson(const FString& _Path, TSharedPtr<FJsonObject> _Content, TFunction<void(bool, int32, TSharedPtr<FJsonObject>)> _OnResponseCompleted)
{
	if (!_Content.IsValid())
	{
		_OnResponseCompleted(false, -1, ErrorMessageJson(TEXT("Content is invalid.")));
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
		_OnResponseCompleted(false, -1, ErrorMessageJson(TEXT("Failed to create a request.")));
	}
}

void UProVRHttpRequest::PutFile(const FString& _Path, const TArray<uint8>& _Content, TFunction<void(bool, int32, TSharedPtr<FJsonObject>)> _OnResponseCompleted)
{
	if (_Content.Num() == 0)
	{
		_OnResponseCompleted(false, -1, ErrorMessageJson(TEXT("Content is invalid.")));
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
		_OnResponseCompleted(false, -1, ErrorMessageJson(TEXT("Failed to create a request.")));
	}
}

UProVRHttpRequest* UProVRHttpRequest::CreateInternalRequest(const FString& _Path, TFunction<void(bool, int32, TSharedPtr<FJsonObject>)> _OnResponseCompleted)
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
		OnResponseCompleted(false, -1, ErrorMessageJson(TEXT("Failed to initialize the request.")));

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
			OnResponseCompleted(false, -1, ErrorMessageJson(TEXT("Request was not successful.")));
		}
		else
		{
			int32 ResponseCode = Response->GetResponseCode();

			if (ResponseCode >= 500) //Retry in case an internal server error
			{
				if (++InternalErrorRetriedCount >= INTERNAL_ERROR_RETRY_TIMES)
				{
					//Exhausted
					OnResponseCompleted(false, ResponseCode, ErrorMessageJson("Exhausted after retrying " + FString::FromInt(INTERNAL_ERROR_RETRY_TIMES) + " times: " + Response->GetContentAsString()));
				}
				else
				{
					RetryRequest();
					return; //Return here, so the request will not be removed from NetworkManager at the end.
				}
			}
			else if (ResponseCode == 401) //Retry in case the token is expired
			{
				NetworkManager->TryRenewingAuthToken([this](bool bSuccess)
					{
						if (bSuccess)
						{
							RetryRequest();
						}
						else
						{
							OnResponseCompleted(false, 401, ErrorMessageJson("Unauthorized."));

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
					OnResponseCompleted(false, ResponseCode, ErrorMessageJson("Failed to deserialize the response: " + Response->GetContentAsString()));
				}
				else if (!EHttpResponseCodes::IsOk(ResponseCode))
				{
					OnResponseCompleted(false, ResponseCode, JsonObject);
				}
				else
				{
					OnResponseCompleted(true, ResponseCode, JsonObject);
				}
			}
		}

		NetworkManager->RemoveHttpRequest(this); //Remove the request from NetworkManager
	}
	else
	{
		OnResponseCompleted(false, -1, ErrorMessageJson(TEXT("Failed to get reference to NetworkManager.")));
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