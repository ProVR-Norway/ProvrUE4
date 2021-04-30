// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "Managers/ProVRManagerBase.h"
#include "ProVRViewManager.generated.h"

UENUM(BlueprintType)
enum class EProVRView : uint8
{
	None = 0,
	Welcome = 1,
	LoginView = 2,
	Register = 3,
	Logout = 4,
	ModelView = 5,
	PauseMenu = 6,
	MainMenu = 7,
	SessionView = 8,
	CreateSessionView = 9,
	OptionsMenu = 10,
	ParticipantHUD = 11
};

UCLASS()
class PROVR_API UProVRViewManager : public UProVRManagerBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TMap<EProVRView, class UProVRWidgetBase*> ViewWidgetMap;

	EProVRView CurrentView = EProVRView::None;

	void SwitchView(EProVRView NextView);
	friend class UProVRSwitchViewAction;

public:
	UProVRViewManager();

	virtual void Tick(float DeltaTime) override;
};
