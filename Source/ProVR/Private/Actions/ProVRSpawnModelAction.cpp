// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRSpawnModelAction.h"
#include "Runtime/Online/HTTP/Public/Http.h"


EProVRActionBehavior UProVRSpawnModelAction::PerformAction()
{

	//Completed Called when GetFromURL http request is finished, see: glTFRuntimeFunctionalLibrary.cpp and IHttpRequest.h

/*
	Completed.BindDynamic(this, BindLambda([this]() {
		UE_LOG(LogTemp, Warning, TEXT("Delegate and Lambda worked"));
		ActionComplete.Broadcast();
		OnAsyncronousActionCompleted();
		}));
	*/

	//How should I make sure that this function returns Synchronous when the delegate "Completed" is called

		//Completed.BindUFunction( this, "DownloadComplete"); // struggling with blue print

	FglTFRuntimeHttpResponse event;
	event.BindDynamic([this](UglTFRuntimeAsset* Asset)
		{
			if (!Asset)
			{
				OnActionComplete.Broadcast(false);
			}
			else
			{
				OnActionComplete.Broadcast(true);
			}

			OnAsyncronousActionCompleted();
		});

	UglTFRuntimeFunctionLibrary::glTFLoadAssetFromUrl(URL, Headers, event, LoaderConfig);

	return EProVRActionBehavior::Asynchronous;

}

/*
void UProVRSpawnModelAction::DownloadComplete()
{
	UE_LOG(LogTemp, Warning, TEXT("UProVRSpawnModelAction::DownloadComplete() is running"));
	ActionComplete.Broadcast();
	OnAsyncronousActionCompleted();
}
*/
