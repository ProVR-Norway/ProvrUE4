// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#include "ProVRPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Components/WidgetComponent.h"

AProVRPawn::AProVRPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	HeadTarget = CreateDefaultSubobject<USceneComponent>(TEXT("GeadTarget"));
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	WidgetInteractionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComp"));
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));

	CapsuleComp->SetupAttachment(RootComponent);
		VROrigin->SetupAttachment(CapsuleComp);
			CameraComp->SetupAttachment(VROrigin);
				HeadTarget->SetupAttachment(CameraComp);
		SkeletalMeshComp->SetupAttachment(VROrigin);
			WidgetInteractionComp->AttachTo(SkeletalMeshComp, "raycast_finger");
		WidgetComp->SetupAttachment(VROrigin);
}

// Called when the game starts or when spawned
void AProVRPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AProVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}