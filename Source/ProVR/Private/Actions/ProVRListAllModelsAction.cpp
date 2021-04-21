// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Actions/ProVRListAllModelsAction.h"
#include "Network/ProVRHttpRequest.h"
#include "GenericPlatform/GenericPlatformHttp.h"

#define CAD_SERVICE_LIST_ALL_MODELS_PATH FString(TEXT("/cadmodels/listall/"))

EProVRActionBehavior UProVRListAllModelsAction::PerformAction()
{
    FString UrlEncodedUsername = FGenericPlatformHttp::UrlEncode(Username);

    FString FullPath = CAD_SERVICE_LIST_ALL_MODELS_PATH + UrlEncodedUsername;
    UProVRHttpRequest::Get(FullPath, [this](int32 HttpResponseCode, TSharedPtr<FJsonObject> HttpResponseContent)
        {
            if (EHttpResponseCodes::IsOk(HttpResponseCode))
            {
                ModelList = HttpResponseContent->GetArrayField("models");
                OnActionCompleteListAllModels.Broadcast(EProVRListAllModelsActionResult::ENUM_OK);
                UE_LOG(LogTemp, Warning, TEXT("Get signed URL action: on action complete: 200"));
            }
            else if (HttpResponseCode == HTTP_UNEXPECTED_ERROR || HttpResponseCode >= 500)
            {
                OnActionCompleteListAllModels.Broadcast(EProVRListAllModelsActionResult::ENUM_InternalError);
                UE_LOG(LogTemp, Warning, TEXT("Get signed URL action: on action complete: 500"));
            }
            else if (HttpResponseCode == 401)
            {
                OnActionCompleteListAllModels.Broadcast(EProVRListAllModelsActionResult::ENUM_Unauthorized);
                UE_LOG(LogTemp, Warning, TEXT("Get signed URL action: on action complete: 401"));
            }
            else if (HttpResponseCode == 404) //Bad Request
            {
                OnActionCompleteListAllModels.Broadcast(EProVRListAllModelsActionResult::ENUM_UserDoesNotExists);
                UE_LOG(LogTemp, Warning, TEXT("Get signed URL action: on action complete: 404"));
            }
            else
            {
                if (HttpResponseContent->HasTypedField<EJson::String>("message"))
                {
                    UE_LOG(LogTemp, Error, TEXT("%s"), *HttpResponseContent->GetStringField("message"));
                }
                OnActionCompleteListAllModels.Broadcast(EProVRListAllModelsActionResult::ENUM_OtherError);
            }
            OnAsyncronousActionCompleted();
        });

    return EProVRActionBehavior::Asynchronous;
}
