// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "ProVRSessionInterface.generated.h"

/**
 * 
 */
UCLASS()
class PROVR_API UProVRSessionInterface : public UObject
{
	GENERATED_BODY()




	UFUNCTION(BlueprintCallable, Category = "ProVR|SessionInterface")
	void CreateSession(FString SessionName,  FString MapName, int32 MaxPlayers);

	void OnCreateSessionComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	UFUNCTION(BlueprintCallable, Category = "ProVR|SessionInterface")
	void SearchSessions();

	UFUNCTION(BlueprintCallable, Category = "ProVR|SessionInterface")
	void JoinSession(FString SessionName);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreateSessionCompleteDelegate, bool, Success, FString, Message);
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSearchSessionCompleteDelegate, bool, Success, FString, );
	FOnSearchSessionCompleteDelegate OnSearchSessionCompleteDelegate;

};
