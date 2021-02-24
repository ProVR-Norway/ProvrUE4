// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "ProVRGameInstance.h"
#include "Managers/ProVRActionManager.h"
#include "Managers/ProVRViewManager.h"
#include "Managers/ProVRNetworkManager.h"

UProVRActionManager* UProVRGameInstance::GetActionManager() const
{
	return ActionManager;
}

UProVRViewManager* UProVRGameInstance::GetViewManager() const
{
	return ViewManager;
}

UProVRNetworkManager* UProVRGameInstance::GetNetworkManager() const
{
	return NetworkManager;
}

UProVRGameInstance::UProVRGameInstance()
{
	if (HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject)) return;

	static ConstructorHelpers::FClassFinder<UProVRActionManager> BPActionManager(TEXT("/Game/Blueprints/Managers/BP_ProVRActionManager"));
	if (BPActionManager.Class != NULL)
	{
		ActionManager = NewObject<UProVRActionManager>(this, BPActionManager.Class, TEXT("ActionManager"));
	}

	static ConstructorHelpers::FClassFinder<UProVRViewManager> BPViewManager(TEXT("/Game/Blueprints/Managers/BP_ProVRViewManager"));
	if (BPViewManager.Class != NULL)
	{
		ViewManager = NewObject<UProVRViewManager>(this, BPViewManager.Class, TEXT("ViewManager"));
	}

	static ConstructorHelpers::FClassFinder<UProVRNetworkManager> BPNetworkManager(TEXT("/Game/Blueprints/Managers/BP_ProVRNetworkManager"));
	if (BPNetworkManager.Class != NULL)
	{
		NetworkManager = NewObject<UProVRNetworkManager>(this, BPNetworkManager.Class, TEXT("NetworkManager"));
	}
}

void UProVRGameInstance::Init()
{
	UGameInstance::Init();


}

void UProVRGameInstance::Shutdown()
{


	UGameInstance::Shutdown();
}