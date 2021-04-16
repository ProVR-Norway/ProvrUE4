// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "Managers/ProVRViewManager.h"
#include "Views/ProVRWidgetBase.h"
#include "ProVRPawn.h"
#include "ProVRGameInstance.h"

UProVRViewManager::UProVRViewManager()
{

	
	if (HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject)) return;
	
	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> WelcomeViewAsset(TEXT("/Game/Widgets/UI_ProVR_WelcomeView"));
	if (WelcomeViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::Welcome, NewObject<UProVRWidgetBase>(this, WelcomeViewAsset.Class, TEXT("UI_ProVR_WelcomeView")));
	}

	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> RegisterViewAsset(TEXT("/Game/Widgets/UI_ProVR_RegisterView"));
	if (RegisterViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::Register, NewObject<UProVRWidgetBase>(this, RegisterViewAsset.Class, TEXT("UI_ProVR_RegisterView")));
	}

	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> LoginViewAsset(TEXT("/Game/Widgets/UI_ProVR_LoginView"));
	if (LoginViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::Login, NewObject<UProVRWidgetBase>(this, LoginViewAsset.Class, TEXT("UI_ProVR_LoginView")));
	}

	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> LogoutViewAsset(TEXT("/Game/Widgets/UI_ProVR_LogoutView"));
	if (LogoutViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::Logout, NewObject<UProVRWidgetBase>(this, LogoutViewAsset.Class, TEXT("UI_ProVR_LogoutView")));
	}


	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> ModelViewAsset(TEXT("/Game/Widgets/UI_ProVR_ModelView"));
	if (ModelViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::ModelView, NewObject<UProVRWidgetBase>(this, ModelViewAsset.Class, TEXT("UI_ProVR_ModelView")));
	}
	
	/*
	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> RegisterViewAsset(TEXT("/Game/Widgets/UI_ProVR_RegisterView"));
	if (RegisterViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::Register, RegisterViewAsset);
	}

	static ConstructorHelpers::FClassFinder<UProVRWidgetBase> LoginViewAsset(TEXT("/Game/Widgets/UI_ProVR_LoginView"));
	if (LoginViewAsset.Class != NULL)
	{
		ViewWidgetMap.Add(EProVRView::Login,  LoginViewAsset);
	}
	*/
}

void UProVRViewManager::Tick(float DeltaTime)
{
	UProVRManagerBase::Tick(DeltaTime);


}

void UProVRViewManager::SwitchView(EProVRView NextView)
{
	
	if (CurrentView != NextView)
	{
		if (ViewWidgetMap.Contains(CurrentView))
		{
			ViewWidgetMap[CurrentView]->RemoveFromParent();
		}

		//ViewWidgetMap[NextView]->AddToViewport();

		if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
		{
			GameInstance->GetPawn()->WidgetComp->SetWidget(ViewWidgetMap[NextView]);
		}
		CurrentView = NextView;
	}
	
	
	
	//if (UProVRGameInstance* GameInstance = GetCurrentGameInstance())
	/*
	if (NextView == EProVRView::Register )
	{
			AProVRPawn::WidgetComp->SetWidget(ViewWidgetMap[NextView]);
	}
	*/
}