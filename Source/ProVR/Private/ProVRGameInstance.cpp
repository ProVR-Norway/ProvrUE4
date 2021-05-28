// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "ProVRGameInstance.h"
#include "Managers/ProVRActionManager.h"
#include "Managers/ProVRViewManager.h"
#include "Managers/ProVRNetworkManager.h"
#include "Misc/NetworkVersion.h"
#include "ProVRPawn.h"

UProVRGameInstance* UProVRGameInstance::GetCurrentGameInstance()
{
	if (GameInstanceWeakPtr.IsValid())
	{
		return GameInstanceWeakPtr.Get();
	}
	return nullptr;
}
TWeakObjectPtr<UProVRGameInstance> UProVRGameInstance::GameInstanceWeakPtr;

UProVRActionManager* UProVRGameInstance::GetActionManager()
{
	if (UProVRGameInstance* GameInstance = GetCurrentGameInstance())
	{
		return GameInstance->ActionManager;
	}
	return nullptr;
}

UProVRViewManager* UProVRGameInstance::GetViewManager()
{
	if (UProVRGameInstance* GameInstance = GetCurrentGameInstance())
	{
		return GameInstance->ViewManager;
	}
	return nullptr;
}

UProVRNetworkManager* UProVRGameInstance::GetNetworkManager()
{
	if (UProVRGameInstance* GameInstance = GetCurrentGameInstance())
	{
		return GameInstance->NetworkManager;
	}
	return nullptr;
}


UProVRGameInstance::UProVRGameInstance()
{
	if (HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject)) return;

	GameInstanceWeakPtr = this;

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
	FNetworkVersion::IsNetworkCompatibleOverride.BindLambda([](uint32 LocalNetworkVersion, uint32 RemoteNetworkVersion)
		{
			return true;
		});
}

void UProVRGameInstance::Init()
{
	UGameInstance::Init();


}

void UProVRGameInstance::Shutdown()
{


	UGameInstance::Shutdown();
}