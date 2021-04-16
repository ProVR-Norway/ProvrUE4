// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "ProVRPawn.h"
#include "Components/WidgetInteractionComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"


// Sets default values
AProVRPawn::AProVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetInteractionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComp"));
	MotionControllerCompRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerCompRight"));
	MotionControllerCompLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerCompLeft"));
	SkeletalMeshComponentRight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshCompRight"));
	SkeletalMeshComponentLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshCompLeft"));
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));


	SceneComp->SetupAttachment(RootComponent);
	MotionControllerCompRight->SetupAttachment(SceneComp);
	MotionControllerCompLeft->SetupAttachment(SceneComp);
	CameraComp->SetupAttachment(SceneComp);
	WidgetComp->SetupAttachment(CameraComp);
	SkeletalMeshComponentRight->SetupAttachment(MotionControllerCompRight);
	SkeletalMeshComponentLeft->SetupAttachment(MotionControllerCompLeft);
	WidgetInteractionComp->SetupAttachment(SkeletalMeshComponentRight);

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

// Called to bind functionality to input
void AProVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

