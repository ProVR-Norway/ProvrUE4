// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRFindSessionAction.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FProVRSessionsOverview {

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FString SessionName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FString MapName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int64 maxParticipants;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int64 ParticipantCount;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FString HostUsername;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FString HostIP;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int64 HostPort;

};
UCLASS()
class PROVR_API UProVRFindSessionAction : public UProVRActionBase
{
	GENERATED_BODY()
public:
	virtual EProVRActionBehavior PerformAction();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateSessionCompleteDelegate, bool, Success);
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
};


 
