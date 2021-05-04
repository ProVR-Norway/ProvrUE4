// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProVRSessionInterface.generated.h"

/**
 * 
 */
UCLASS()
class PROVR_API UProVRSessionInterface : public UObject
{
	GENERATED_BODY()

	virtual void Init();


	UFUNCTION(BlueprintCallable, Category = "ProVR|SessionInterface")
	void CreateSession(FString SessionName,  FString MapName, uint32 MaxPlayers);

	void OnCreateSessionComplete(bool Success, FString Content);


	UFUNCTION(BlueprintCallable, Category = "ProVR|SessionInterface")
	void SearchSessions();

	UFUNCTION(BlueprintCallable, Category = "ProVR|SessionInterface")
	void JoinSession(FString SessionName);

	DECLARE_DYNAMIC_DELEGATE_TwoParams(FProVRRequestDoneDelegate, bool, Success, FString, Content);
		FProVRRequestDoneDelegate OnSessionCreateComplete;

};
