// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/WidgetComponent.h"
#include "ProVRPawn.generated.h"

//class UWidgetComponent;
class UWidgetInteractionComponent;
class UCameraComponent;
class UMotionControllerComponent;
class USkeletalMeshComponent;
class USceneComponent;


UCLASS()
class PROVR_API AProVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AProVRPawn();

	UPROPERTY(EditAnywhere, Category = "ProVR|Actions")
		UWidgetComponent* WidgetComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		UWidgetInteractionComponent* WidgetInteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		UMotionControllerComponent* MotionControllerCompRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		UMotionControllerComponent* MotionControllerCompLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		USkeletalMeshComponent* SkeletalMeshComponentRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		USkeletalMeshComponent* SkeletalMeshComponentLeft;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
