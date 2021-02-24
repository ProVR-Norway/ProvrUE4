// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "Actions/ProVRGoogleHttpRequestAction.h"
#include "Runtime/Online/HTTP/Public/Http.h"

bool UProVRGoogleHttpRequestAction::PerformAction()
{
	auto Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (!bWasSuccessful)
			{
			}
		});

	Request->SetURL("https://google.com");
	Request->SetVerb("GET");

	return Request->ProcessRequest();
}