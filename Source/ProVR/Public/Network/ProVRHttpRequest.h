// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "ProVRHttpRequest.generated.h"

enum class EHttpRequestType : uint8
{
	ENUM_Get,
	ENUM_Post,
	ENUM_Put,
	ENUM_Delete
};

#define HTTP_UNEXPECTED_ERROR -1

UCLASS()
class PROVR_API UProVRHttpRequest : public UObject
{
	GENERATED_BODY()
	
private:
	EHttpRequestType RequestType;
	FString RequestFinalUrl;
	TArray<uint8> RequestContent;

	//int32: Http Response Code, TSharedPtr<class FJsonObject> JsonContent
	TFunction<void(int32, TSharedPtr<class FJsonObject>)> OnResponseCompleted;

	TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> InternalHttpRequest;

	static UProVRHttpRequest* CreateInternalRequest(const FString& _Path, TFunction<void(int32, TSharedPtr<class FJsonObject>)> _OnResponseCompleted);
	void ProcessInternalRequest();

	void OnInternalRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	int32 InternalErrorRetriedCount = 0;

	void RetryRequest();

	static TSharedPtr<class FJsonObject> ErrorMessageJson(const FString& Message);

public:
	static void Get(const FString& _Path, TFunction<void(int32, TSharedPtr<class FJsonObject>)> _OnResponseCompleted);
	static void Delete(const FString& _Path, TFunction<void(int32, TSharedPtr<class FJsonObject>)> _OnResponseCompleted);
	static void PostJson(const FString& _Path, TSharedPtr<class FJsonObject> _Content, TFunction<void(int32, TSharedPtr<class FJsonObject>)> _OnResponseCompleted);
	static void PutJson(const FString& _Path, TSharedPtr<class FJsonObject> _Content, TFunction<void(int32, TSharedPtr<class FJsonObject>)> _OnResponseCompleted);
	static void PutFile(const FString& _Path, const TArray<uint8>& _Content, TFunction<void(int32, TSharedPtr<class FJsonObject>)> _OnResponseCompleted);

	FORCEINLINE EHttpRequestType GetRequestType()
	{
		return RequestType;
	}
};
