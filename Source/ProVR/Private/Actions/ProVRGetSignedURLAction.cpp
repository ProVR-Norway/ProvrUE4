// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "Actions/ProVRGetSignedURLAction.h"
#include "Network/ProVRHttpRequest.h"
#include "GenericPlatform/GenericPlatformHttp.h"

#define CAD_SERVICE_GET_SIGNED_URL_PATH FString(TEXT("/cadmodels/signedurl/"))

EProVRActionBehavior UProVRGetSignedURLAction::PerformAction()
{
    FString UrlEncodedUsername = FGenericPlatformHttp::UrlEncode(Username);
    FString UrlEncodedModelName = FGenericPlatformHttp::UrlEncode(ModelName);

    FString FullPath = CAD_SERVICE_GET_SIGNED_URL_PATH + UrlEncodedUsername + '/' + UrlEncodedModelName;
    UProVRHttpRequest::Get(FullPath, [this](int32 HttpResponseCode, TSharedPtr<FJsonObject> HttpResponseContent)
        {
            if (EHttpResponseCodes::IsOk(HttpResponseCode))
            {
                OnActionCompleteSignedURL.Broadcast(EProVRGetSignedURLActionResult::ENUM_OK);
                UE_LOG(LogTemp, Warning, TEXT("Get signed URL action: on action complete: 200"));
                SignedURL = HttpResponseContent->GetStringField("signedURL");
                UE_LOG(LogTemp, Warning, TEXT("%s"), *SignedURL);
            }
            else if (HttpResponseCode == HTTP_UNEXPECTED_ERROR || HttpResponseCode >= 500)
            {
                OnActionCompleteSignedURL.Broadcast(EProVRGetSignedURLActionResult::ENUM_InternalError);
                UE_LOG(LogTemp, Warning, TEXT("Get signed URL action: on action complete: 500"));
            }
            else if (HttpResponseCode == 401)
            {
                OnActionCompleteSignedURL.Broadcast(EProVRGetSignedURLActionResult::ENUM_Unauthorized);
                UE_LOG(LogTemp, Warning, TEXT("Get signed URL action: on action complete: 401"));
            }
            else if (HttpResponseCode == 400) //Bad Request
            {
                OnActionCompleteSignedURL.Broadcast(EProVRGetSignedURLActionResult::ENUM_UserDoesNotExists);
                UE_LOG(LogTemp, Warning, TEXT("Get signed URL action: on action complete: 400"));
            }
            else
            {
                if (HttpResponseContent->HasTypedField<EJson::String>("message"))
                {
                    UE_LOG(LogTemp, Error, TEXT("%s"), *HttpResponseContent->GetStringField("message"));
                }
                OnActionCompleteSignedURL.Broadcast(EProVRGetSignedURLActionResult::ENUM_OtherError);
            }
            OnAsyncronousActionCompleted();
        });

    return EProVRActionBehavior::Asynchronous;
}