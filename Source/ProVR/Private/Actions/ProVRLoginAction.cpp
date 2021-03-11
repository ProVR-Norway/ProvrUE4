// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "Actions/ProVRLoginAction.h"
#include "ProVRGameInstance.h"
#include "Managers/ProVRNetworkManager.h"

EProVRActionBehavior UProVRLoginAction::PerformAction()
{
	if (UProVRNetworkManager* NetworkManager = UProVRGameInstance::GetNetworkManager())
	{
		NetworkManager->PerformLoginRequest(EmailAddress, Password, [this](int32 LoginHttpResponseCode)
			{
				if (EHttpResponseCodes::IsOk(LoginHttpResponseCode))
				{
					OnActionComplete.Broadcast(EProVRLoginActionResult::ENUM_OK);
				}
				else if (LoginHttpResponseCode == HTTP_UNEXPECTED_ERROR || LoginHttpResponseCode >= 500)
				{
					OnActionComplete.Broadcast(EProVRLoginActionResult::ENUM_ServerError);
				}
				else if (LoginHttpResponseCode == 404) //Not Found
				{
					OnActionComplete.Broadcast(EProVRLoginActionResult::ENUM_UserDoesNotExists);
				}
				else if (LoginHttpResponseCode == 400) //Bad Request
				{
					OnActionComplete.Broadcast(EProVRLoginActionResult::ENUM_InvalidEmailOrPass);
				}
				else
				{
					OnActionComplete.Broadcast(EProVRLoginActionResult::ENUM_OtherError);
				}

				OnAsyncronousActionCompleted();
			});

		return EProVRActionBehavior::Asynchronous;
	}

	OnActionComplete.Broadcast(EProVRLoginActionResult::ENUM_OtherError);
	return EProVRActionBehavior::Synchronous;
}