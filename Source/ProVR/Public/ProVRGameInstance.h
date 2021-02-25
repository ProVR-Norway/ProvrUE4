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

	static TWeakObjectPtr<UProVRGameInstance> GameInstanceWeakPtr;
	
public:
	UFUNCTION(BlueprintPure, Category = "ProVR|Managers")
	static class UProVRActionManager* GetActionManager();

	UFUNCTION(BlueprintPure, Category = "ProVR|Managers")
	static class UProVRViewManager* GetViewManager();

	UFUNCTION(BlueprintPure, Category = "ProVR|Managers")
	static class UProVRNetworkManager* GetNetworkManager();

	static UProVRGameInstance* GetCurrentGameInstance();

	UProVRGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;
};