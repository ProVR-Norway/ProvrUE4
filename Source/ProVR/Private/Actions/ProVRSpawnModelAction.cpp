// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRSpawnModelAction.h"
#include "Runtime/Online/HTTP/Public/Http.h"


void UProVRSpawnModelAction::Event(UglTFRuntimeAsset* Asset_)
{
	if (!Asset)
	{
		OnActionComplete.Broadcast(false);
	}
	else
	{
		OnActionComplete.Broadcast(true);
	}

	UE_LOG(LogTemp, Warning, TEXT("Event() Completed"));
	OnAsyncronousActionCompleted();
}


EProVRActionBehavior UProVRSpawnModelAction::PerformAction()
{

	Completed.BindUFunction(this, "Event");

	/*
	FglTFRuntimeHttpResponse event;
	
	event.BindDynamic(this, [this](UglTFRuntimeAsset* Asset)
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
		*/
	UglTFRuntimeFunctionLibrary::glTFLoadAssetFromUrl(URL, Headers, Completed, LoaderConfig);

	return EProVRActionBehavior::Asynchronous;

}


