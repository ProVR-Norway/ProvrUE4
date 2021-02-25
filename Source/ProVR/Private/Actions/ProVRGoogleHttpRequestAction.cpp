// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "Actions/ProVRGoogleHttpRequestAction.h"
#include "Runtime/Online/HTTP/Public/Http.h"

EProVRActionBehavior UProVRGoogleHttpRequestAction::PerformAction()
{
	auto Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (!bWasSuccessful || !Response.IsValid())
			{
				OnActionComplete.Broadcast(false, TEXT("Request was not successful."));
			}
			else if (!EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				OnActionComplete.Broadcast(false, Response->GetContentAsString());
			}
			else
			{
				OnActionComplete.Broadcast(true, Response->GetContentAsString());
			}

			OnAsyncronousActionCompleted();
		});

	Request->SetURL("https://google.com");
	Request->SetVerb("GET");

	if (!Request->ProcessRequest()) //Fails to initialize the request
	{
		return EProVRActionBehavior::Synchronous;
	}
	return EProVRActionBehavior::Asynchronous;
}