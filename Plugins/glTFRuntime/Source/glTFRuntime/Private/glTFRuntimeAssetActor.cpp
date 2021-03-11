// Copyright 2020, Roberto De Ioris.


#include "glTFRuntimeAssetActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Animation/AnimSequence.h"

// Sets default values
AglTFRuntimeAssetActor::AglTFRuntimeAssetActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AssetRoot = CreateDefaultSubobject<USceneComponent>(TEXT("AssetRoot"));
	RootComponent = AssetRoot;
}

// Called when the game starts or when spawned
void AglTFRuntimeAssetActor::BeginPlay()
{
	Super::BeginPlay();

	if (!Asset)
	{
		return;
	}

	TArray<FglTFRuntimeScene> Scenes = Asset->GetScenes();
	for (FglTFRuntimeScene& Scene : Scenes)
	{
		USceneComponent* SceneComponent = NewObject<USceneComponent>(this, *FString::Printf(TEXT("Scene %d"), Scene.Index));
		SceneComponent->SetupAttachment(RootComponent);
		SceneComponent->RegisterComponent();
		AddInstanceComponent(SceneComponent);
		for (int32 NodeIndex : Scene.RootNodesIndices)
		{
			FglTFRuntimeNode Node;
			if (!Asset->GetNode(NodeIndex, Node))
			{
				return;
			}
			ProcessNode(SceneComponent, Node);
		}
	}
}

void AglTFRuntimeAssetActor::ProcessNode(USceneComponent* NodeParentComponent, FglTFRuntimeNode& Node)
{
	// skip bones/joints
	if (Asset->NodeIsBone(Node.Index))
	{
		return;
	}

	USceneComponent* NewComponent = nullptr;
	if (Node.CameraIndex != INDEX_NONE)
	{
		UCameraComponent* NewCameraComponent = NewObject<UCameraComponent>(this, *Node.Name);
		NewCameraComponent->SetupAttachment(NodeParentComponent);
		NewCameraComponent->RegisterComponent();
		NewCameraComponent->SetRelativeTransform(Node.Transform);
		AddInstanceComponent(NewCameraComponent);
		Asset->LoadCamera(Node.CameraIndex, NewCameraComponent);
		NewComponent = NewCameraComponent;

	}
	else if (Node.MeshIndex < 0)
	{
		NewComponent = NewObject<USceneComponent>(this, *Node.Name);
		NewComponent->SetupAttachment(NodeParentComponent);
		NewComponent->RegisterComponent();
		NewComponent->SetRelativeTransform(Node.Transform);
		AddInstanceComponent(NewComponent);
	}
	else
	{
		if (Node.SkinIndex < 0)
		{
			UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(this, *Node.Name);
			StaticMeshComponent->SetupAttachment(NodeParentComponent);
			StaticMeshComponent->RegisterComponent();
			StaticMeshComponent->SetRelativeTransform(Node.Transform);
			AddInstanceComponent(StaticMeshComponent);
			if (StaticMeshConfig.Outer == nullptr)
			{
				StaticMeshConfig.Outer = StaticMeshComponent;
			}
			UStaticMesh* StaticMesh = Asset->LoadStaticMesh(Node.MeshIndex, StaticMeshConfig);
			if (StaticMesh && !StaticMeshConfig.ExportOriginalPivotToSocket.IsEmpty())
			{
				UStaticMeshSocket* DeltaSocket = StaticMesh->FindSocket(FName(StaticMeshConfig.ExportOriginalPivotToSocket));
				if (DeltaSocket)
				{
					FTransform NewTransform = StaticMeshComponent->GetRelativeTransform();
					FVector DeltaLocation = -DeltaSocket->RelativeLocation * NewTransform.GetScale3D();
					DeltaLocation = NewTransform.GetRotation().RotateVector(DeltaLocation);
					NewTransform.AddToTranslation(DeltaLocation);
					StaticMeshComponent->SetRelativeTransform(NewTransform);
				}
			}
			StaticMeshComponent->SetStaticMesh(StaticMesh);
			ReceiveOnStaticMeshComponentCreated(StaticMeshComponent, Node);
			NewComponent = StaticMeshComponent;
		}
		else
		{
			USkeletalMeshComponent* SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(this, *Node.Name);
			SkeletalMeshComponent->SetupAttachment(NodeParentComponent);
			SkeletalMeshComponent->RegisterComponent();
			SkeletalMeshComponent->SetRelativeTransform(Node.Transform);
			AddInstanceComponent(SkeletalMeshComponent);
			USkeletalMesh* SkeletalMesh = Asset->LoadSkeletalMesh(Node.MeshIndex, Node.SkinIndex, SkeletalMeshConfig);
			SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
			ReceiveOnSkeletalMeshComponentCreated(SkeletalMeshComponent, Node);
			NewComponent = SkeletalMeshComponent;
		}
	}

	if (!NewComponent)
	{
		return;
	}

	// check for animations
	if (!NewComponent->IsA<USkeletalMeshComponent>())
	{
		TArray<UglTFRuntimeAnimationCurve*> ComponentAnimationCurves = Asset->LoadAllNodeAnimationCurves(Node.Index);
		TMap<FString, UglTFRuntimeAnimationCurve*> ComponentAnimationCurvesMap;
		for (UglTFRuntimeAnimationCurve* ComponentAnimationCurve : ComponentAnimationCurves)
		{
			if (!CurveBasedAnimations.Contains(NewComponent))
			{
				CurveBasedAnimations.Add(NewComponent, ComponentAnimationCurve);
				CurveBasedAnimationsTimeTracker.Add(NewComponent, 0);
			}
			DiscoveredCurveAnimationsNames.Add(ComponentAnimationCurve->glTFCurveAnimationName);
			ComponentAnimationCurvesMap.Add(ComponentAnimationCurve->glTFCurveAnimationName, ComponentAnimationCurve);
		}
		DiscoveredCurveAnimations.Add(NewComponent, ComponentAnimationCurvesMap);
	}
	else
	{
		USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(NewComponent);
		FglTFRuntimeSkeletalAnimationConfig SkeletalAnimationConfig;
		UAnimSequence* SkeletalAnimation = Asset->LoadNodeSkeletalAnimation(SkeletalMeshComponent->SkeletalMesh, Node.Index, SkeletalAnimationConfig);
		if (SkeletalAnimation)
		{
			SkeletalMeshComponent->AnimationData.AnimToPlay = SkeletalAnimation;
			SkeletalMeshComponent->AnimationData.bSavedLooping = true;
			SkeletalMeshComponent->AnimationData.bSavedPlaying = true;
			SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		}
	}

	for (int32 ChildIndex : Node.ChildrenIndices)
	{
		FglTFRuntimeNode Child;
		if (!Asset->GetNode(ChildIndex, Child))
		{
			return;
		}
		ProcessNode(NewComponent, Child);
	}
}

void AglTFRuntimeAssetActor::SetCurveAnimationByName(const FString& CurveAnimationName)
{
	if (!DiscoveredCurveAnimationsNames.Contains(CurveAnimationName))
	{
		return;
	}

	for (TPair<USceneComponent*, UglTFRuntimeAnimationCurve*>& Pair : CurveBasedAnimations)
	{

		TMap<FString, UglTFRuntimeAnimationCurve*> WantedCurveAnimationsMap = DiscoveredCurveAnimations[Pair.Key];
		if (WantedCurveAnimationsMap.Contains(CurveAnimationName))
		{
			Pair.Value = WantedCurveAnimationsMap[CurveAnimationName];
			CurveBasedAnimationsTimeTracker[Pair.Key] = 0;
		}
		else
		{
			Pair.Value = nullptr;
		}

	}

}

// Called every frame
void AglTFRuntimeAssetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (TPair<USceneComponent*, UglTFRuntimeAnimationCurve*>& Pair : CurveBasedAnimations)
	{
		// the curve could be null
		if (!Pair.Value)
		{
			continue;
		}
		float MinTime;
		float MaxTime;
		Pair.Value->GetTimeRange(MinTime, MaxTime);

		float CurrentTime = CurveBasedAnimationsTimeTracker[Pair.Key];
		if (CurrentTime > Pair.Value->glTFCurveAnimationDuration)
		{
			CurveBasedAnimationsTimeTracker[Pair.Key] = 0;
			CurrentTime = 0;
		}

		if (CurrentTime >= MinTime)
		{
			FTransform FrameTransform = Pair.Value->GetTransformValue(CurveBasedAnimationsTimeTracker[Pair.Key]);
			Pair.Key->SetRelativeTransform(FrameTransform);
		}
		CurveBasedAnimationsTimeTracker[Pair.Key] += DeltaTime;
	}
}

void AglTFRuntimeAssetActor::ReceiveOnStaticMeshComponentCreated_Implementation(UStaticMeshComponent* StaticMeshComponent, const FglTFRuntimeNode& Node)
{

}

void AglTFRuntimeAssetActor::ReceiveOnSkeletalMeshComponentCreated_Implementation(USkeletalMeshComponent* SkeletalMeshComponent, const FglTFRuntimeNode& Node)
{

}
