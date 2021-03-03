// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Actions/ProVRActionBase.h"
#include "glTFRuntimeFunctionLibrary.h"
#include "ProVRSpawnModelAction.generated.h"

/**
 *
 */




UCLASS()
class PROVR_API UProVRSpawnModelAction : public UProVRActionBase
{
	GENERATED_BODY()


public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProVRSpawnModelComplete, bool, bSuccessful);
	UPROPERTY(BlueprintReadWrite, Category = "ProVR|Actions")
		FProVRSpawnModelComplete OnActionComplete;
	/*
		UPROPERTY(BlueprintReadWrite, Category = "ProVR|Actions")
			FglTFRuntimeHttpResponse Completed;
		*/


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
		FString URL;

	//Possibility to add extra header info on HttpRequest
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
		TMap<FString, FString> Headers;

	//Enum, constructior takes no paramenters
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
		FglTFRuntimeConfig LoaderConfig;


	EProVRActionBehavior PerformAction();


	//protected:
	//void DownloadComplete();
};
