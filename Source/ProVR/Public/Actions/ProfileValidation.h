// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <string>
#include "ProfileValidation.generated.h"

/**
 *
 */
UCLASS()
class PROVR_API UProfileValidation : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
		static bool IsEmailValid(const FString& email);

	UFUNCTION(BlueprintPure)
		static FString PasswordStrength(const FString& password);

private:
	
};