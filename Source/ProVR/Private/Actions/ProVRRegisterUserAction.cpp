// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "Actions/ProVRRegisterUserAction.h"
#include "Network/ProVRHttpRequest.h"
#include "Dom/JsonObject.h"

#define AUTH_SERVICE_REGISTER_REQUEST_PATH FString(TEXT("/auth/register"))

EProVRActionBehavior UProVRRegisterUserAction::PerformAction()
{
	TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject);
	RequestJson->SetStringField("emailAddress", EmailAddress);
	RequestJson->SetStringField("password", Password);

	UProVRHttpRequest::PostJson(AUTH_SERVICE_REGISTER_REQUEST_PATH, RequestJson, 
		[this](int32 HttpResponseCode, TSharedPtr<FJsonObject> HttpResponseContent)
		{
			if (EHttpResponseCodes::IsOk(HttpResponseCode))
			{
				OnActionComplete.Broadcast(EProVRRegisterUserActionResult::ENUM_OK);
			}
			else if (HttpResponseCode == HTTP_UNEXPECTED_ERROR || HttpResponseCode >= 500)
			{
				OnActionComplete.Broadcast(EProVRRegisterUserActionResult::ENUM_ServerError);
			}
			else if (HttpResponseCode == 409) //Conflict
			{
				OnActionComplete.Broadcast(EProVRRegisterUserActionResult::ENUM_UserAlreadyExists);
			}
			else if (HttpResponseCode == 400) //Bad Request
			{
				OnActionComplete.Broadcast(EProVRRegisterUserActionResult::ENUM_InvalidEmailOrPass);
			}
			else
			{
				if (HttpResponseContent->HasTypedField<EJson::String>("message"))
				{
					UE_LOG(LogTemp, Error, TEXT("%s"), *HttpResponseContent->GetStringField("message"));
				}
				
				OnActionComplete.Broadcast(EProVRRegisterUserActionResult::ENUM_OtherError);
			}

			OnAsyncronousActionCompleted();
		});

	return EProVRActionBehavior::Asynchronous;
}