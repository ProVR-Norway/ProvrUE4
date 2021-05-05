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
	// Not possible with two Loadings form URL at the same time????
	
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProVRSpawnModelComplete, bool, bSuccessful);
	UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
		FProVRSpawnModelComplete OnActionComplete;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
		FString URL;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
		TMap<FString, FString> Headers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
		FglTFRuntimeConfig LoaderConfig;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
		UglTFRuntimeAsset* Asset;

	FglTFRuntimeHttpResponse Completed;

	UFUNCTION()
		void Event(UglTFRuntimeAsset* Asset_);

	EProVRActionBehavior PerformAction();


	
	//protected:
	//void DownloadComplete();
};
