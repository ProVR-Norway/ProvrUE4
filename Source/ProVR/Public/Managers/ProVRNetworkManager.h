// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Managers/ProVRManagerBase.h"
#include "Network/ProVRHttpRequest.h"
#include "ProVRNetworkManager.generated.h"

#define BACKEND_BASE_URL FString(TEXT("https://api-gateway-iu3tuzfidq-ez.a.run.app"))
#define INTERNAL_ERROR_RETRY_TIMES 3

/*
Path to login request in the auth service.

Service endpoint should expect a json object like:
{
	"emailAddress": "email@address.com",
	"password": "......"
}

Service endpoint should return a json object like: (in case of success)
{
	"token": "Basic ........."
}
*/
#define AUTH_SERVICE_LOGIN_REQUEST_PATH FString(TEXT("/auth/login"))

UCLASS()
class PROVR_API UProVRNetworkManager : public UProVRManagerBase
{
	GENERATED_BODY()
	
private:
	//https://en.wikipedia.org/wiki/Basic_access_authentication
	FString CurrentAuthToken;

	FString LastUsername;
	FString LastPassword;

	UPROPERTY()
	TArray<class UProVRHttpRequest*> ActiveHttpRequests;

	TArray<TFunction<void(int32)>> OngoingTryRenewingAuthTokenRequestSubscribers;
	bool bTryRenewingAuthTokenRequestIsOngoing = false;
	TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> OngoingTryRenewingAuthTokenRequest;
	bool bWasOngoingTryRenewingAuthTokenRequestCanceled = false;
	void CallSubscribersAfterTryRenewingAuthTokenResponse(int32 HttpResponseCode);

	void OnRenewingAuthTokenRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

public:
	FORCEINLINE FString GetCurrentAuthToken()
	{
		return CurrentAuthToken;
	}

	void PerformLoginRequest(const FString& Username, const FString& Password, TFunction<void(int32)> OnCompleted);
	void TryRenewingAuthToken(TFunction<void(int32)> OnCompleted);

	void PushNewHttpRequest(class UProVRHttpRequest* NewHttpRequest);
	void RemoveHttpRequest(class UProVRHttpRequest* HttpRequest);
};
