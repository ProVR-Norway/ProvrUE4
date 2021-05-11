// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/WidgetComponent.h"
#include "ProVRPawn.generated.h"

class UCapsuleComponent;
class USceneComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UWidgetInteractionComponent;
class UWidgetComponent;

UCLASS()
class PROVR_API AProVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AProVRPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		USceneComponent* VROrigin;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		USceneComponent* HeadTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		UWidgetInteractionComponent* WidgetInteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		UWidgetComponent* WidgetComp;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
