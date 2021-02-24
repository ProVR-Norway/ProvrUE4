// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "Actions/ProVRHelloWorldAction.h"

bool UProVRHelloWorldAction::PerformAction()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Hello world!"));
		return true;
	}
	return false;
}