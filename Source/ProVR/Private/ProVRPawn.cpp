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
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	/*
	WidgetInteractionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComp"));
	MotionControllerCompRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerCompRight"));
	MotionControllerCompLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerCompLeft"));
	SkeletalMeshComponentRight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshCompRight"));
	SkeletalMeshComponentLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshCompLeft"));
	*/

	SceneComp->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SceneComp);
	WidgetComp->SetupAttachment(SceneComp);
	/*
	MotionControllerCompRight->SetupAttachment(SceneComp);
	MotionControllerCompLeft->SetupAttachment(SceneComp);
	SkeletalMeshComponentRight->SetupAttachment(MotionControllerCompRight);
	SkeletalMeshComponentLeft->SetupAttachment(MotionControllerCompLeft);
	WidgetInteractionComp->SetupAttachment(SkeletalMeshComponentRight);
	*/
}

// Called when the game starts or when spawned
void AProVRPawn::BeginPlay()
{
	Super::BeginPlay();

}
/*
void AProVRPawn::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AProVRPawn::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}
*/
/*
void AProVRPawn::TestInput()
{
	UE_LOG(LogTemp, Warning, TEXT("AProVRPawn::TestInput() called!!!"));
}
*/
// Called every frame
void AProVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void AProVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	/*
	//PlayerInputComponent->BindAction("TEST", this, IE_Pressed, &AProVRPawn::Test);
	PlayerInputComponent->BindAxis("MoveForward", this, &AProVRPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProVRPawn::MoveRight);
	PlayerInputComponent->BindAction("test123", IE_Pressed, this, &AProVRPawn::TestInput);
	PlayerInputComponent->BindAction("MotionControllerThumbLeft_Y", IE_Pressed, this, &AProVRPawn::MotionControllerThumbLeft_Y);
	PlayerInputComponent->BindAction("MotionControllerThumbLeft_X", IE_Pressed, this, &AProVRPawn::MotionControllerThumbLeft_X);
	PlayerInputComponent->BindAction("MotionControllerThumbRight_Y", IE_Pressed, this, &AProVRPawn::MotionControllerThumbRight_Y);
	PlayerInputComponent->BindAction("MotionControllerThumbRight_X", IE_Pressed, this, &AProVRPawn::MotionControllerThumbRight_X);
	PlayerInputComponent->BindAction("GrabLeft", IE_Pressed, this, &AProVRPawn::GrabLeft);
	PlayerInputComponent->BindAction("GrabRight", IE_Pressed, this, &AProVRPawn::GrabRight);
	PlayerInputComponent->BindAction("ScaleUp", IE_Pressed, this, &AProVRPawn::ScaleUp);
	PlayerInputComponent->BindAction("ScaleDown", IE_Pressed, this, &AProVRPawn::ScaleDown);
	PlayerInputComponent->BindAction("ScaleDefault", IE_Pressed, this, &AProVRPawn::ScaleDefault);
	PlayerInputComponent->BindAction("SpawnButton", IE_Pressed, this, &AProVRPawn::SpawnButton);
	PlayerInputComponent->BindAction("DeleteButton", IE_Pressed, this, &AProVRPawn::DeleteButton);
	PlayerInputComponent->BindAction("WidgetInteraction", IE_Pressed, this, &AProVRPawn::WidgetInteraction);
	PlayerInputComponent->BindAction("EnableRotation", IE_Pressed, this, &AProVRPawn::EnableRotation);
	*/
}

