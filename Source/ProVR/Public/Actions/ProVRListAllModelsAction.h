// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Actions/ProVRActionBase.h"
#include "ProVRListAllModelsAction.generated.h"

UENUM(BlueprintType)
enum class EProVRListAllModelsActionResult : uint8
{
    ENUM_OK                     UMETA(DisplayName = "All models returned successfully"),
    ENUM_UserDoesNotExists      UMETA(DisplayName = "User Does Not Exist"),
    ENUM_Unauthorized           UMETA(DisplayName = "Unauthorized. Please re-login"),
    ENUM_InternalError          UMETA(DisplayName = "Internal error"),
    ENUM_OtherError             UMETA(DisplayName = "Other error"),
};

USTRUCT(BlueprintType)
struct FProVRUserModel {

    GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    FString ModelName;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    FString DateUploaded;
};

UCLASS()
class PROVR_API UProVRListAllModelsAction : public UProVRActionBase
{
    GENERATED_BODY()

public:
    virtual EProVRActionBehavior PerformAction() override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
    FString Username;

    // Note: TSharedPtr cannot be Blueprint editable
    // Note: TArray cannot be nested ("Nested containers are not supported.")

    
    // HOW DO WE MAKE SURE THIS CAN BE ACCESSED IN BLUEPRINTS?
    //UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ProVR|Actions")
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
    TArray<FProVRUserModel> UserModelList;
    
    /*
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProVR|Actions")
    TArray<Fstring> ModelsDateUploaded;
    */

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProVRListAllModelsActionDoneDelegate, EProVRListAllModelsActionResult, Result);
    UPROPERTY(BlueprintAssignable, Category = "ProVR|Actions")
    FProVRListAllModelsActionDoneDelegate OnActionCompleteListAllModels;
};

