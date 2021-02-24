// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "ProVRPawn.h"

// Sets default values
AProVRPawn::AProVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

