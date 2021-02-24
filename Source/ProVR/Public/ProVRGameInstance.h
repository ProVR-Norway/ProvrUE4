// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProVRGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROVR_API UProVRGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	class UProVRActionManager* ActionManager;

	UPROPERTY()
	class UProVRViewManager* ViewManager;

	UPROPERTY()
	class UProVRNetworkManager* NetworkManager;

public:
	UFUNCTION(BlueprintCallable, Category = "ProVR|Managers")
	class UProVRActionManager* GetActionManager() const;

	UFUNCTION(BlueprintCallable, Category = "ProVR|Managers")
	class UProVRViewManager* GetViewManager() const;

	UFUNCTION(BlueprintCallable, Category = "ProVR|Managers")
	class UProVRNetworkManager* GetNetworkManager() const;

	UProVRGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;
};