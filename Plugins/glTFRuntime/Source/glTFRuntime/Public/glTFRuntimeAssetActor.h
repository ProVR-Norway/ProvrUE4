// Copyright 2020, Roberto De Ioris.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "glTFRuntimeAsset.h"
#include "glTFRuntimeAssetActor.generated.h"

UCLASS()
class GLTFRUNTIME_API AglTFRuntimeAssetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AglTFRuntimeAssetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ProcessNode(USceneComponent* NodeParentComponent, FglTFRuntimeNode& Node);

	TMap<USceneComponent*, float>  CurveBasedAnimationsTimeTracker;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "glTFRuntime")
	TSet<FString> DiscoveredCurveAnimationsNames;

	TMap<USceneComponent*, TMap<FString, UglTFRuntimeAnimationCurve*>> DiscoveredCurveAnimations;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "glTFRuntime")
	UglTFRuntimeAsset* Asset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "glTFRuntime")
	FglTFRuntimeStaticMeshConfig StaticMeshConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "glTFRuntime")
	FglTFRuntimeSkeletalMeshConfig SkeletalMeshConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TMap<USceneComponent*, UglTFRuntimeAnimationCurve*> CurveBasedAnimations;

	UFUNCTION(BlueprintNativeEvent, Category = "glTFRuntime", meta = (DisplayName = "On StaticMeshComponent Created"))
	void ReceiveOnStaticMeshComponentCreated(UStaticMeshComponent* StaticMeshComponent, const FglTFRuntimeNode& Node);

	UFUNCTION(BlueprintNativeEvent, Category = "glTFRuntime", meta = (DisplayName = "On SkeletalMeshComponent Created"))
	void ReceiveOnSkeletalMeshComponentCreated(USkeletalMeshComponent* SkeletalMeshComponent, const FglTFRuntimeNode& Node);

	UFUNCTION(BlueprintCallable, Category = "glTFRuntime")
	void SetCurveAnimationByName(const FString& CurveAnimationName);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="glTFRuntime")
	USceneComponent* AssetRoot;

};
