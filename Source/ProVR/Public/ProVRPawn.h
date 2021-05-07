// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/WidgetComponent.h"
#include "ProVRMovementController.h"
#include "ProVRPawn.generated.h"

//class UWidgetComponent;
class UWidgetInteractionComponent;
class UCameraComponent;
class UMotionControllerComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UProVRMovementController;
class UCapsuleComponent;



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
		UProVRMovementController* PawnMovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		UWidgetComponent* WidgetComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		UMotionControllerComponent* LeftHandControllerComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		UMotionControllerComponent* RightHandControllerComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		UWidgetInteractionComponent* WidgetInteractionComp;

	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		UMotionControllerComponent* MotionControllerCompRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		UMotionControllerComponent* MotionControllerCompLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		USkeletalMeshComponent* SkeletalMeshComponentRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProVR|Actions")
		USkeletalMeshComponent* SkeletalMeshComponentLeft;
		*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	void MoveForward(float Value);

	void MoveRight(float Value);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "TestInput"))
		void TestInput();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "MotionControllerThumbLeft_Y"))
		void MotionControllerThumbLeft_Y();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "MotionControllerThumbLeft_X"))
		void MotionControllerThumbLeft_X();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "MotionControllerThumbRight_Y"))
		void MotionControllerThumbRight_Y();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "MotionControllerThumbRight_X"))
		void MotionControllerThumbRight_X();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "GrabLeft"))
		void GrabLeft();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "GrabRight"))
		void GrabRight();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ScaleUp"))
		void ScaleUp();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ScaleDown"))
		void ScaleDown();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ScaleDefault"))
		void ScaleDefault();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "SpawnButton"))
		void SpawnButton();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "DeleteButton"))
		void DeleteButton();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "WidgetInteraction"))
		void WidgetInteraction();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "EnableRotation"))
		void EnableRotation();
		*/

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
