// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Network/ProVRSessionInterface.h"
#include "CoreMinimal.h"


void UProVRSessionInterface::Init()
{
	OnSessionCreateComplete.BindUFunction(this, "OnCreateSessionComplete");
}

void UProVRSessionInterface::CreateSession(FString SessionName, FString MapName, uint32 MaxPlayers)
{


}

void UProVRSessionInterface::OnCreateSessionComplete(bool Success, FString Content)
{
	
}


void UProVRSessionInterface::SearchSessions(bool Success, FString Content)
{
}

void UProVRSessionInterface::JoinSession(FString SessionName)
{
}
