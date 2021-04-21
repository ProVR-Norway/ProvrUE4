// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRListAllModelsAction.generated.h"

UENUM(BlueprintType)
enum class EProVRListAllModelsActionResult : uint8
{
    ENUM_OK                     UMETA(DisplayName = "Signed URL is successfully generated"),
    ENUM_UserDoesNotExists      UMETA(DisplayName = "User Does Not Exist"),
    ENUM_Unauthorized           UMETA(DisplayName = "Unauthorized. Please re-login"),
    ENUM_InternalError          UMETA(DisplayName = "Internal error"),
    ENUM_OtherError             UMETA(DisplayName = "Other error"),
};

UCLASS()
class PROVR_API UProVRListAllModelsAction : public UProVRActionBase
{
    GENERATED_BODY()

public:
    virtual EProVRActionBehavior PerformAction() override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
    FString Username;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
    TArray<TArray<FString>> ModelList;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProVRListAllModelsActionDoneDelegate, EProVRListAllModelsActionResult, Result);
    UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
    FProVRListAllModelsActionDoneDelegate OnActionCompleteListAllModels;
};

