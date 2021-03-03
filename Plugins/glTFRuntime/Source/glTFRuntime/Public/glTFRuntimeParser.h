// Copyright 2020, Roberto De Ioris.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonValue.h"
#include "Dom/JsonObject.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "Camera/CameraComponent.h"
#include "glTFRuntimeAnimationCurve.h"
#include "ProceduralMeshComponent.h"
#if WITH_EDITOR
#include "Rendering/SkeletalMeshLODImporterData.h"
#endif
#include "glTFRuntimeParser.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGLTFRuntime, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FglTFRuntimeError, const FString, ErrorContext, const FString, ErrorMessage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FglTFRuntimeOnStaticMeshCreated, UStaticMesh*, StaticMesh);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FglTFRuntimeOnSkeletalMeshCreated, USkeletalMesh*, SkeletalMesh);

UENUM()
enum class EglTFRuntimeTransformBaseType : uint8
{
	Default,
	Matrix,
	Transform,
	YForward,
	BasisMatrix,
	Identity,
};

USTRUCT(BlueprintType)
struct FglTFRuntimeBasisMatrix
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FVector XAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FVector YAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FVector ZAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FVector Origin;

	FMatrix GetMatrix() const
	{
		return FBasisVectorMatrix(XAxis, YAxis, ZAxis, Origin);
	}
};

USTRUCT(BlueprintType)
struct FglTFRuntimeConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	EglTFRuntimeTransformBaseType TransformBaseType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FMatrix BasisMatrix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FTransform BaseTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FglTFRuntimeBasisMatrix BasisVectorMatrix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	float SceneScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TArray<FString> ContentPluginsToScan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bAllowExternalFiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FString OverrideBaseDirectory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bOverrideBaseDirectoryFromContentDir;

	bool bSearchContentDir;

	FglTFRuntimeConfig()
	{
		TransformBaseType = EglTFRuntimeTransformBaseType::Default;
		BasisMatrix = FMatrix::Identity;
		BaseTransform = FTransform::Identity;
		SceneScale = 100;
		bSearchContentDir = false;
		bAllowExternalFiles = false;
		bOverrideBaseDirectoryFromContentDir = false;
	}

	FMatrix GetMatrix() const
	{
		const FMatrix DefaultMatrix = FBasisVectorMatrix(FVector(0, 0, -1), FVector(1, 0, 0), FVector(0, 1, 0), FVector::ZeroVector);

		if (TransformBaseType == EglTFRuntimeTransformBaseType::Default)
		{
			return DefaultMatrix;
		}

		if (TransformBaseType == EglTFRuntimeTransformBaseType::Matrix)
		{
			return BasisMatrix;
		}

		if (TransformBaseType == EglTFRuntimeTransformBaseType::BasisMatrix)
		{
			return BasisVectorMatrix.GetMatrix();
		}

		if (TransformBaseType == EglTFRuntimeTransformBaseType::Transform)
		{
			return BaseTransform.ToMatrixWithScale();
		}

		if (TransformBaseType == EglTFRuntimeTransformBaseType::YForward)
		{
			return FBasisVectorMatrix(FVector(1, 0, 0), FVector(0, 0, 1), FVector(0, 1, 0), FVector::ZeroVector);
		}

		if (TransformBaseType == EglTFRuntimeTransformBaseType::Identity)
		{
			return FMatrix::Identity;
		}

		return DefaultMatrix;
	}
};

USTRUCT(BlueprintType)
struct FglTFRuntimeScene
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "glTFRuntime")
	int32 Index;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "glTFRuntime")
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "glTFRuntime")
	TArray<int32> RootNodesIndices;
};


USTRUCT(BlueprintType)
struct FglTFRuntimeNode
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "glTFRuntime")
	int32 Index;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "glTFRuntime")
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "glTFRuntime")
	FTransform Transform;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "glTFRuntime")
	int32 MeshIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "glTFRuntime")
	int32 SkinIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "glTFRuntime")
	int32 CameraIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "glTFRuntime")
	TArray<int32> ChildrenIndices;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "glTFRuntime")
	int32 ParentIndex;

	FglTFRuntimeNode()
	{
		Index = INDEX_NONE;
		Transform = FTransform::Identity;
		ParentIndex = INDEX_NONE;
		MeshIndex = INDEX_NONE;
		SkinIndex = INDEX_NONE;
		CameraIndex = INDEX_NONE;
	}
};

UENUM()
enum class EglTFRuntimeMaterialType : uint8
{
	Opaque,
	Translucent,
	TwoSided,
	TwoSidedTranslucent,
};

UENUM()
enum class EglTFRuntimeCacheMode : uint8
{
	ReadWrite,
	None,
	Read,
	Write
};

UENUM()
enum class EglTFRuntimePivotPosition : uint8
{
	Asset,
	Center,
	Top,
	Bottom
};

USTRUCT(BlueprintType)
struct FglTFRuntimeSocket
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FString BoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FTransform Transform;
};

USTRUCT(BlueprintType)
struct FglTFRuntimeBone
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FString BoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	int32 ParentIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FTransform Transform;
};

USTRUCT(BlueprintType)
struct FglTFRuntimeMorphTarget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TArray<FVector> Positions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TArray<FVector> Normals;
};

USTRUCT(BlueprintType)
struct FglTFRuntimeMaterialsConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	EglTFRuntimeCacheMode CacheMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TMap<EglTFRuntimeMaterialType, UMaterialInterface*> UberMaterialsOverrideMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TMap<int32, UMaterialInterface*> MaterialsOverrideMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TMap<int32, UTexture2D*> TexturesOverrideMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TMap<int32, UTexture2D*> ImagesOverrideMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bDisableVertexColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bGeneratesMipMaps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bMergeSectionsByMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	float SpecularFactor;

	FglTFRuntimeMaterialsConfig()
	{
		CacheMode = EglTFRuntimeCacheMode::ReadWrite;
		bGeneratesMipMaps = false;
		bMergeSectionsByMaterial = false;
		SpecularFactor = 0;
		bDisableVertexColors = false;
	}
};

USTRUCT(BlueprintType)
struct FglTFRuntimeStaticMeshConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	EglTFRuntimeCacheMode CacheMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bReverseWinding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bBuildSimpleCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TArray<FBox> BoxCollisions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TArray<FVector4> SphereCollisions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TEnumAsByte<ECollisionTraceFlag> CollisionComplexity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bAllowCPUAccess;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	EglTFRuntimePivotPosition PivotPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	UObject* Outer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FglTFRuntimeMaterialsConfig MaterialsConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TMap<FString, FTransform> Sockets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FString ExportOriginalPivotToSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TMap<int32, float> LODScreenSize;

	FglTFRuntimeStaticMeshConfig()
	{
		CacheMode = EglTFRuntimeCacheMode::ReadWrite;
		bReverseWinding = false;
		bBuildSimpleCollision = false;
		Outer = nullptr;
		CollisionComplexity = ECollisionTraceFlag::CTF_UseDefault;
		bAllowCPUAccess = true;
		PivotPosition = EglTFRuntimePivotPosition::Asset;
	}
};

USTRUCT(BlueprintType)
struct FglTFRuntimeProceduralMeshConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bReverseWinding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bBuildSimpleCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TArray<FBox> BoxCollisions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TArray<FVector4> SphereCollisions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bUseComplexAsSimpleCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	EglTFRuntimePivotPosition PivotPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FglTFRuntimeMaterialsConfig MaterialsConfig;

	FglTFRuntimeProceduralMeshConfig()
	{
		bReverseWinding = false;
		bBuildSimpleCollision = false;
		bUseComplexAsSimpleCollision = false;
		PivotPosition = EglTFRuntimePivotPosition::Asset;
	}
};

USTRUCT(BlueprintType)
struct FglTFRuntimeSkeletonConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	EglTFRuntimeCacheMode CacheMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bAddRootBone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FString RootBoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TMap<FString, FString> BonesNameMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TMap<FString, FTransform> BonesTransformMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bNormalizeSkeletonScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	int32 RootNodeIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TMap<FString, FglTFRuntimeSocket> Sockets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bClearRotations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	USkeleton* CopyRotationsFrom;

	FglTFRuntimeSkeletonConfig()
	{
		CacheMode = EglTFRuntimeCacheMode::ReadWrite;
		RootNodeIndex = INDEX_NONE;
		bNormalizeSkeletonScale = false;
		bAddRootBone = false;
		bClearRotations = false;
		CopyRotationsFrom = nullptr;
	}
};

USTRUCT(BlueprintType)
struct FglTFRuntimeCapsule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FVector Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	float Length;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FRotator Rotation;

	FglTFRuntimeCapsule()
	{
		Radius = 0;
		Length = 0;
	}
};

USTRUCT(BlueprintType)
struct FglTFRuntimePhysicsBody
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TEnumAsByte<ECollisionTraceFlag> CollisionTraceFlag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TEnumAsByte<EPhysicsType> PhysicsType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bConsiderForBounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TArray<FglTFRuntimeCapsule> CapsuleCollisions;

	FglTFRuntimePhysicsBody()
	{
		CollisionTraceFlag = ECollisionTraceFlag::CTF_UseDefault;
		PhysicsType = EPhysicsType::PhysType_Default;
		bConsiderForBounds = true;
	}
};

USTRUCT(BlueprintType)
struct FglTFRuntimeSkeletalMeshConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	EglTFRuntimeCacheMode CacheMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	USkeleton* Skeleton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bOverwriteRefSkeleton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bMergeAllBonesToBoneTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TArray<FglTFRuntimeBone> CustomSkeleton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bIgnoreSkin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	int32 OverrideSkinIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FglTFRuntimeSkeletonConfig SkeletonConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FglTFRuntimeMaterialsConfig MaterialsConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TMap<int32, float> LODScreenSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FVector BoundsScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bShiftBoundsByRootBone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bIgnoreMissingBones;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	TMap<FString, FglTFRuntimePhysicsBody> PhysicsBodies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	UObject* Outer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FString SaveToPackage;

	FglTFRuntimeSkeletalMeshConfig()
	{
		CacheMode = EglTFRuntimeCacheMode::ReadWrite;
		bOverwriteRefSkeleton = false;
		Skeleton = nullptr;
		bIgnoreSkin = false;
		OverrideSkinIndex = -1;
		BoundsScale = FVector::OneVector;
		bShiftBoundsByRootBone = false;
		bIgnoreMissingBones = false;
		bMergeAllBonesToBoneTree = false;
		Outer = nullptr;
	}
};

USTRUCT(BlueprintType)
struct FglTFRuntimeSkeletalAnimationConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	EglTFRuntimeCacheMode CacheMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	int32 RootNodeIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bRootMotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bRemoveRootMotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bRemoveTranslations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bRemoveRotations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bRemoveScales;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	bool bRemoveMorphTargets;

	FglTFRuntimeSkeletalAnimationConfig()
	{
		RootNodeIndex = INDEX_NONE;
		bRootMotion = false;
		bRemoveRootMotion = false;
		CacheMode = EglTFRuntimeCacheMode::ReadWrite;
		bRemoveTranslations = false;
		bRemoveRotations = false;
		bRemoveScales = false;
		bRemoveMorphTargets = false;
	}
};

struct FglTFRuntimeUInt16Vector4
{
	uint16 X;
	uint16 Y;
	uint16 Z;
	uint16 W;

	FglTFRuntimeUInt16Vector4()
	{
		X = 0;
		Y = 0;
		Z = 0;
		W = 0;
	}

	uint16& operator[](int32 Index)
	{
		check(Index >= 0 && Index < 4);
		switch (Index)
		{
		case 0:
			return X;
		case 1:
			return Y;
		case 2:
			return Z;
		case 3:
		default:
			return W;
		}
		;
	}
};

struct FglTFRuntimePrimitive
{
	TArray<FVector> Positions;
	TArray<FVector> Normals;
	TArray<FVector4> Tangents;
	TArray<TArray<FVector2D>> UVs;
	TArray<uint32> Indices;
	UMaterialInterface* Material;
	TArray<TArray<FglTFRuntimeUInt16Vector4>> Joints;
	TArray<TArray<FVector4>> Weights;
	TArray<FVector4> Colors;
	TArray<FglTFRuntimeMorphTarget> MorphTargets;
	TMap<int32, FName> OverrideBoneMap;
	TMap<int32, int32> BonesCache;
};

USTRUCT(BlueprintType)
struct FglTFRuntimeLOD
{
	GENERATED_BODY()

	TArray<FglTFRuntimePrimitive> Primitives;

	bool bHasNormals;
	bool bHasTangents;
	bool bHasUV;

#if WITH_EDITOR
	FSkeletalMeshImportData ImportData;
#endif

	FglTFRuntimeLOD()
	{
		bHasNormals = false;
		bHasTangents = false;
		bHasUV = false;
	}
};

struct FglTFRuntimeSkeletalMeshContext : public FGCObject
{
	TSharedRef<class FglTFRuntimeParser> Parser;

	const FglTFRuntimeSkeletalMeshConfig SkeletalMeshConfig;

	USkeletalMesh* SkeletalMesh;

	TArray<FglTFRuntimeLOD> LODs;

	int32 SkinIndex;

	FBox BoundingBox;

	FglTFRuntimeSkeletalMeshContext(TSharedRef<FglTFRuntimeParser> InParser, const FglTFRuntimeSkeletalMeshConfig& InSkeletalMeshConfig) : Parser(InParser), SkeletalMeshConfig(InSkeletalMeshConfig)
	{
		EObjectFlags Flags = RF_Public;
		UObject* Outer = InSkeletalMeshConfig.Outer ? InSkeletalMeshConfig.Outer : GetTransientPackage();
#if WITH_EDITOR
		if (!InSkeletalMeshConfig.SaveToPackage.IsEmpty())
		{
			if (FindPackage(nullptr, *InSkeletalMeshConfig.SaveToPackage) || LoadPackage(nullptr, *InSkeletalMeshConfig.SaveToPackage, RF_Public|RF_Standalone))
			{
				UE_LOG(LogGLTFRuntime, Error, TEXT("UPackage %s already exists. Falling back to Transient."), *InSkeletalMeshConfig.SaveToPackage);
				Outer = GetTransientPackage();
			}
			else
			{
#if ENGINE_MINOR_VERSION >= 26
				Outer = CreatePackage(*InSkeletalMeshConfig.SaveToPackage);
#else
				Outer = CreatePackage(nullptr, *InSkeletalMeshConfig.SaveToPackage);
#endif
				if (!Outer)
				{
					UE_LOG(LogGLTFRuntime, Error, TEXT("Unable to get UPackage %s. Falling back to Transient."), *InSkeletalMeshConfig.SaveToPackage);
					Outer = GetTransientPackage();
				}
				else
				{
					Flags |= RF_Standalone;
				}
			}
		}
#endif
		SkeletalMesh = NewObject<USkeletalMesh>(Outer, NAME_None, Flags);
		BoundingBox = FBox(EForceInit::ForceInitToZero);
		SkinIndex = -1;
	}

	void AddReferencedObjects(FReferenceCollector& Collector)
	{
		Collector.AddReferencedObject(SkeletalMesh);
	}
};

struct FglTFRuntimeMipMap
{
	const int32 TextureIndex;
	TArray64<uint8> Pixels;
	int32 Width;
	int32 Height;

	FglTFRuntimeMipMap(const int32 InTextureIndex) : TextureIndex(InTextureIndex)
	{
		Width = 0;
		Height = 0;
	}
};

struct FglTFRuntimeMaterial
{
	bool bTwoSided;
	bool bTranslucent;
	float AlphaCutoff;
	EglTFRuntimeMaterialType MaterialType;

	bool bHasBaseColorFactor;
	FLinearColor BaseColorFactor;

	TArray<FglTFRuntimeMipMap> BaseColorTextureMips;
	UTexture2D* BaseColorTextureCache;
	int32 BaseColorTexCoord;

	bool bHasMetallicFactor;
	double MetallicFactor;
	bool bHasRoughnessFactor;
	double RoughnessFactor;

	TArray<FglTFRuntimeMipMap> MetallicRoughnessTextureMips;
	UTexture2D* MetallicRoughnessTextureCache;
	int32 MetallicRoughnessTexCoord;

	TArray<FglTFRuntimeMipMap> NormalTextureMips;
	UTexture2D* NormalTextureCache;
	int32 NormalTexCoord;

	TArray<FglTFRuntimeMipMap> OcclusionTextureMips;
	UTexture2D* OcclusionTextureCache;
	int32 OcclusionTexCoord;

	bool bHasEmissiveFactor;
	FLinearColor EmissiveFactor;

	TArray<FglTFRuntimeMipMap> EmissiveTextureMips;
	UTexture2D* EmissiveTextureCache;
	int32 EmissiveTexCoord;

	bool bHasSpecularFactor;
	FLinearColor SpecularFactor;

	bool bHasGlossinessFactor;
	double GlossinessFactor;

	TArray<FglTFRuntimeMipMap> SpecularGlossinessTextureMips;
	UTexture2D* SpecularGlossinessTextureCache;
	int32 SpecularGlossinessTexCoord;

	float BaseSpecularFactor;

	bool bHasDiffuseFactor;
	FLinearColor DiffuseFactor;

	TArray<FglTFRuntimeMipMap> DiffuseTextureMips;
	UTexture2D* DiffuseTextureCache;
	int32 DiffuseTexCoord;

	bool bKHR_materials_pbrSpecularGlossiness;

	FglTFRuntimeMaterial()
	{
		bTwoSided = false;
		bTranslucent = false;
		AlphaCutoff = 0;
		MaterialType = EglTFRuntimeMaterialType::Opaque;
		bHasBaseColorFactor = false;
		BaseColorTextureCache = nullptr;
		BaseColorTexCoord = 0;
		bHasMetallicFactor = false;
		bHasRoughnessFactor = false;
		MetallicRoughnessTextureCache = nullptr;
		MetallicRoughnessTexCoord = 0;
		NormalTextureCache = nullptr;
		NormalTexCoord = 0;
		OcclusionTextureCache = nullptr;
		OcclusionTexCoord = 0;
		bHasEmissiveFactor = false;
		EmissiveTextureCache = nullptr;
		EmissiveTexCoord = 0;
		bHasSpecularFactor = false;
		bHasGlossinessFactor = false;
		SpecularGlossinessTextureCache = nullptr;
		SpecularGlossinessTexCoord = 0;
		BaseSpecularFactor = 0;
		bHasDiffuseFactor = false;
		DiffuseTextureCache = nullptr;
		DiffuseTexCoord = 0;
		bKHR_materials_pbrSpecularGlossiness = false;
	}
};

USTRUCT(BlueprintType)
struct FglTFRuntimePathItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	FString Path;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "glTFRuntime")
	int32 Index;

	FglTFRuntimePathItem()
	{
		Index = INDEX_NONE;
	}
};


DECLARE_DYNAMIC_DELEGATE_OneParam(FglTFRuntimeSkeletalMeshAsync, USkeletalMesh*, SkeletalMesh);

/**
 *
 */
class GLTFRUNTIME_API FglTFRuntimeParser : public FGCObject, public TSharedFromThis<FglTFRuntimeParser>
{
public:
	FglTFRuntimeParser(TSharedRef<FJsonObject> JsonObject, const FMatrix& InSceneBasis, float InSceneScale);

	static TSharedPtr<FglTFRuntimeParser> FromFilename(const FString& Filename, const FglTFRuntimeConfig& LoaderConfig);
	static TSharedPtr<FglTFRuntimeParser> FromBinary(const uint8* DataPtr, int64 DataNum, const FglTFRuntimeConfig& LoaderConfig);
	static TSharedPtr<FglTFRuntimeParser> FromString(const FString& JsonData, const FglTFRuntimeConfig& LoaderConfig);
	static TSharedPtr<FglTFRuntimeParser> FromData(const uint8* DataPtr, int64 DataNum, const FglTFRuntimeConfig& LoaderConfig);

	static FORCEINLINE TSharedPtr<FglTFRuntimeParser> FromBinary(const TArray<uint8> Data, const FglTFRuntimeConfig& LoaderConfig) { return FromBinary(Data.GetData(), Data.Num(), LoaderConfig); }
	static FORCEINLINE TSharedPtr<FglTFRuntimeParser> FromBinary(const TArray64<uint8> Data, const FglTFRuntimeConfig& LoaderConfig) { return FromBinary(Data.GetData(), Data.Num(), LoaderConfig); }
	static FORCEINLINE TSharedPtr<FglTFRuntimeParser> FromData(const TArray<uint8> Data, const FglTFRuntimeConfig& LoaderConfig) { return FromData(Data.GetData(), Data.Num(), LoaderConfig); }
	static FORCEINLINE TSharedPtr<FglTFRuntimeParser> FromData(const TArray64<uint8> Data, const FglTFRuntimeConfig& LoaderConfig) { return FromData(Data.GetData(), Data.Num(), LoaderConfig); }

	UStaticMesh* LoadStaticMesh(const int32 MeshIndex, const FglTFRuntimeStaticMeshConfig& StaticMeshConfig);
	bool LoadStaticMeshes(TArray<UStaticMesh*>& StaticMeshes, const FglTFRuntimeStaticMeshConfig& StaticMeshConfig);

	TArray<UStaticMesh*> LoadStaticMeshesFromPrimitives(const int32 MeshIndex, const FglTFRuntimeStaticMeshConfig& StaticMeshConfig);

	UStaticMesh* LoadStaticMeshLODs(const TArray<int32> MeshIndices, const FglTFRuntimeStaticMeshConfig& StaticMeshConfig);

	UStaticMesh* LoadStaticMeshByName(const FString MeshName, const FglTFRuntimeStaticMeshConfig& StaticMeshConfig);

	UMaterialInterface* LoadMaterial(const int32 MaterialIndex, const FglTFRuntimeMaterialsConfig& MaterialsConfig, const bool bUseVertexColors);
	UTexture2D* LoadTexture(const int32 TextureIndex, TArray<FglTFRuntimeMipMap>& Mips, const bool sRGB, const FglTFRuntimeMaterialsConfig& MaterialsConfig);

	bool LoadNodes();
	bool LoadNode(int32 NodeIndex, FglTFRuntimeNode& Node);
	bool LoadNodeByName(const FString& NodeName, FglTFRuntimeNode& Node);
	bool LoadNodesRecursive(const int32 NodeIndex, TArray<FglTFRuntimeNode>& Nodes);

	bool LoadScenes(TArray<FglTFRuntimeScene>& Scenes);
	bool LoadScene(int32 SceneIndex, FglTFRuntimeScene& Scene);

	USkeletalMesh* LoadSkeletalMesh(const int32 MeshIndex, const int32 SkinIndex, const FglTFRuntimeSkeletalMeshConfig& SkeletalMeshConfig);
	UAnimSequence* LoadSkeletalAnimation(USkeletalMesh* SkeletalMesh, const int32 AnimationIndex, const FglTFRuntimeSkeletalAnimationConfig& SkeletalAnimationConfig);
	UAnimSequence* LoadSkeletalAnimationByName(USkeletalMesh* SkeletalMesh, const FString AnimationName, const FglTFRuntimeSkeletalAnimationConfig& SkeletalAnimationConfig);
	UAnimSequence* LoadNodeSkeletalAnimation(USkeletalMesh* SkeletalMesh, const int32 NodeIndex, const FglTFRuntimeSkeletalAnimationConfig& SkeletalAnimationConfig);
	USkeleton* LoadSkeleton(const int32 SkinIndex, const FglTFRuntimeSkeletonConfig& SkeletonConfig);

	void LoadSkeletalMeshAsync(const int32 MeshIndex, const int32 SkinIndex, FglTFRuntimeSkeletalMeshAsync AsyncCallback, const FglTFRuntimeSkeletalMeshConfig& SkeletalMeshConfig);

	USkeletalMesh* LoadSkeletalMeshLODs(const TArray<int32> MeshIndices, const int32 SkinIndex, const FglTFRuntimeSkeletalMeshConfig& SkeletalMeshConfig);

	USkeletalMesh* LoadSkeletalMeshRecursive(const FString& NodeName, const int32 SkinIndex, const TArray<FString>& ExcludeNodes, const FglTFRuntimeSkeletalMeshConfig& SkeletalMeshConfig);
	void LoadSkeletalMeshRecursiveAsync(const FString& NodeName, const int32 SkinIndex, const TArray<FString>& ExcludeNodes, FglTFRuntimeSkeletalMeshAsync AsyncCallback, const FglTFRuntimeSkeletalMeshConfig& SkeletalMeshConfig);

	UglTFRuntimeAnimationCurve* LoadNodeAnimationCurve(const int32 NodeIndex);
	TArray<UglTFRuntimeAnimationCurve*> LoadAllNodeAnimationCurves(const int32 NodeIndex);

	bool GetBuffer(int32 BufferIndex, TArray64<uint8>& Bytes);
	bool GetBufferView(int32 BufferViewIndex, TArray64<uint8>& Bytes, int64& Stride);
	bool GetAccessor(int32 AccessorIndex, int64& ComponentType, int64& Stride, int64& Elements, int64& ElementSize, int64& Count, TArray64<uint8>& Bytes);

	bool GetAllNodes(TArray<FglTFRuntimeNode>& Nodes);

	TArray<FString> GetCamerasNames();

	bool LoadCameraIntoCameraComponent(const int32 CameraIndex, UCameraComponent* CameraComponent);

	int64 GetComponentTypeSize(const int64 ComponentType) const;
	int64 GetTypeSize(const FString& Type) const;

	bool ParseBase64Uri(const FString& Uri, TArray64<uint8>& Bytes);

	void AddReferencedObjects(FReferenceCollector& Collector);

	bool LoadPrimitives(TSharedRef<FJsonObject> JsonMeshObject, TArray<FglTFRuntimePrimitive>& Primitives, const FglTFRuntimeMaterialsConfig& MaterialsConfig);
	bool LoadPrimitive(TSharedRef<FJsonObject> JsonPrimitiveObject, FglTFRuntimePrimitive& Primitive, const FglTFRuntimeMaterialsConfig& MaterialsConfig);

	void AddError(const FString& ErrorContext, const FString& ErrorMessage);
	void ClearErrors();

	bool NodeIsBone(const int32 NodeIndex);

	FglTFRuntimeError OnError;
	FglTFRuntimeOnStaticMeshCreated OnStaticMeshCreated;
	FglTFRuntimeOnSkeletalMeshCreated OnSkeletalMeshCreated;

	void SetBinaryBuffer(const TArray64<uint8>& InBinaryBuffer)
	{
		BinaryBuffer = InBinaryBuffer;
	}

	bool LoadStaticMeshIntoProceduralMeshComponent(const int32 MeshIndex, UProceduralMeshComponent* ProceduralMeshComponent, const FglTFRuntimeProceduralMeshConfig& ProceduralMeshConfig);

	USkeletalMesh* FinalizeSkeletalMeshWithLODs(TSharedRef<FglTFRuntimeSkeletalMeshContext, ESPMode::ThreadSafe> SkeletalMeshContext);

	TSharedPtr<FJsonValue> GetJSONObjectFromPath(const TArray<FglTFRuntimePathItem>& Path) const;

	FString GetJSONStringFromPath(const TArray<FglTFRuntimePathItem>& Path, bool& bFound) const;
	double GetJSONNumberFromPath(const TArray<FglTFRuntimePathItem>& Path, bool& bFound) const;
	bool GetJSONBooleanFromPath(const TArray<FglTFRuntimePathItem>& Path, bool& bFound) const;

	int32 GetJSONArraySizeFromPath(const TArray<FglTFRuntimePathItem>& Path, bool& bFound) const;

protected:
	TSharedRef<FJsonObject> Root;

	TMap<int32, UStaticMesh*> StaticMeshesCache;
	TMap<int32, UMaterialInterface*> MaterialsCache;
	TMap<int32, USkeleton*> SkeletonsCache;
	TMap<int32, USkeletalMesh*> SkeletalMeshesCache;
	TMap<int32, UTexture2D*> TexturesCache;

	TMap<int32, TArray64<uint8>> BuffersCache;

	TArray<FglTFRuntimeNode> AllNodesCache;
	bool bAllNodesCached;

	TArray64<uint8> BinaryBuffer;

	UStaticMesh* LoadStaticMesh_Internal(TArray<TSharedRef<FJsonObject>> JsonMeshObjects, const FglTFRuntimeStaticMeshConfig& StaticMeshConfig, const TMap<TSharedRef<FJsonObject>, TArray<FglTFRuntimePrimitive>>& PrimitivesCache);
	UMaterialInterface* LoadMaterial_Internal(TSharedRef<FJsonObject> JsonMaterialObject, const FglTFRuntimeMaterialsConfig& MaterialsConfig, const bool bUseVertexColors);
	bool LoadNode_Internal(int32 Index, TSharedRef<FJsonObject> JsonNodeObject, int32 NodesCount, FglTFRuntimeNode& Node);

	UMaterialInterface* BuildMaterial(const FglTFRuntimeMaterial& RuntimeMaterial, const FglTFRuntimeMaterialsConfig& MaterialsConfig, const bool bUseVertexColors);
	UTexture2D* BuildTexture(UObject* Outer, const TArray<FglTFRuntimeMipMap>& Mips, const TEnumAsByte<TextureCompressionSettings> Compression, const bool sRGB, const FglTFRuntimeMaterialsConfig& MaterialsConfig);

	bool LoadSkeletalAnimation_Internal(TSharedRef<FJsonObject> JsonAnimationObject, TMap<FString, FRawAnimSequenceTrack>& Tracks, TMap<FName, TArray<TPair<float, float>>>& MorphTargetCurves, float& Duration, const FglTFRuntimeSkeletalAnimationConfig& SkeletalAnimationConfig, TFunctionRef<bool(const FglTFRuntimeNode& Node)> Filter);

	bool LoadAnimation_Internal(TSharedRef<FJsonObject> JsonAnimationObject, float& Duration, FString& Name, TFunctionRef<void(const FglTFRuntimeNode& Node, const FString& Path, const TArray<float> Timeline, const TArray<FVector4> Values)> Callback, TFunctionRef<bool(const FglTFRuntimeNode& Node)> NodeFilter);

	USkeletalMesh* CreateSkeletalMeshFromLODs(TSharedRef<FglTFRuntimeSkeletalMeshContext, ESPMode::ThreadSafe> SkeletalMeshContext);

	bool FillReferenceSkeleton(TSharedRef<FJsonObject> JsonSkinObject, FReferenceSkeleton& RefSkeleton, TMap<int32, FName>& BoneMap, const FglTFRuntimeSkeletonConfig& SkeletonConfig);
	bool FillFakeSkeleton(FReferenceSkeleton& RefSkeleton, TMap<int32, FName>& BoneMap, const FglTFRuntimeSkeletalMeshConfig& SkeletalMeshConfig);
	bool TraverseJoints(FReferenceSkeletonModifier& Modifier, int32 Parent, FglTFRuntimeNode& Node, const TArray<int32>& Joints, TMap<int32, FName>& BoneMap, const TMap<int32, FMatrix>& InverseBindMatricesMap, const FglTFRuntimeSkeletonConfig& SkeletonConfig);

	bool GetMorphTargetNames(const int32 MeshIndex, TArray<FName>& MorphTargetNames);

	void FixNodeParent(FglTFRuntimeNode& Node);

	int32 FindCommonRoot(const TArray<int32>& NodeIndices);
	int32 FindTopRoot(int32 NodeIndex);
	bool HasRoot(int32 NodeIndex, int32 RootIndex);

	bool CheckJsonIndex(TSharedRef<FJsonObject> JsonObject, const FString& FieldName, const int32 Index, TArray<TSharedRef<FJsonValue>>& JsonItems);
	bool CheckJsonRootIndex(const FString FieldName, const int32 Index, TArray<TSharedRef<FJsonValue>>& JsonItems) { return CheckJsonIndex(Root, FieldName, Index, JsonItems); }
	TSharedPtr<FJsonObject> GetJsonObjectFromIndex(TSharedRef<FJsonObject> JsonObject, const FString& FieldName, const int32 Index);
	TSharedPtr<FJsonObject> GetJsonObjectFromRootIndex(const FString& FieldName, const int32 Index) { return GetJsonObjectFromIndex(Root, FieldName, Index); }

	FString GetJsonObjectString(TSharedRef<FJsonObject> JsonObject, const FString& FieldName, const FString& DefaultValue);
	int32 GetJsonObjectIndex(TSharedRef<FJsonObject> JsonObject, const FString& FieldName, const int32 DefaultValue);

	bool FillJsonMatrix(const TArray<TSharedPtr<FJsonValue>>* JsonMatrixValues, FMatrix& Matrix);

	float FindBestFrames(const TArray<float>& FramesTimes, float WantedTime, int32& FirstIndex, int32& SecondIndex);

	void NormalizeSkeletonScale(FReferenceSkeleton& RefSkeleton);
	void NormalizeSkeletonBoneScale(FReferenceSkeletonModifier& Modifier, const int32 BoneIndex, FVector BoneScale);

	void ClearSkeletonRotations(FReferenceSkeleton& RefSkeleton);
	void ApplySkeletonBoneRotation(FReferenceSkeletonModifier& Modifier, const int32 BoneIndex, FQuat BoneRotation);

	void CopySkeletonRotationsFrom(FReferenceSkeleton& RefSkeleton, const FReferenceSkeleton& SrcRefSkeleton);

	bool CanReadFromCache(const EglTFRuntimeCacheMode CacheMode) { return CacheMode == EglTFRuntimeCacheMode::Read || CacheMode == EglTFRuntimeCacheMode::ReadWrite; }
	bool CanWriteToCache(const EglTFRuntimeCacheMode CacheMode) { return CacheMode == EglTFRuntimeCacheMode::Write || CacheMode == EglTFRuntimeCacheMode::ReadWrite; }

	FMatrix SceneBasis;
	float SceneScale;

	TMap<EglTFRuntimeMaterialType, UMaterialInterface*> MetallicRoughnessMaterialsMap;
	TMap<EglTFRuntimeMaterialType, UMaterialInterface*> SpecularGlossinessMaterialsMap;

	TArray<FString> Errors;

	FString BaseDirectory;

	template<typename T, typename Callback>
	bool BuildFromAccessorField(TSharedRef<FJsonObject> JsonObject, const FString& Name, TArray<T>& Data, const TArray<int64>& SupportedElements, const TArray<int64>& SupportedTypes, const bool bNormalized, Callback Filter)
	{
		int64 AccessorIndex;
		if (!JsonObject->TryGetNumberField(Name, AccessorIndex))
			return false;

		TArray64<uint8> Bytes;
		int64 ComponentType = 0, Stride = 0, Elements = 0, ElementSize = 0, Count = 0;
		if (!GetAccessor(AccessorIndex, ComponentType, Stride, Elements, ElementSize, Count, Bytes))
		{
			return false;
		}

		if (!SupportedElements.Contains(Elements))
		{
			return false;
		}

		if (!SupportedTypes.Contains(ComponentType))
		{
			return false;
		}

		for (int64 ElementIndex = 0; ElementIndex < Count; ElementIndex++)
		{
			int64 Index = ElementIndex * Stride;
			T Value;
			// FLOAT
			if (ComponentType == 5126)
			{
				float* Ptr = (float*)&(Bytes[Index]);
				for (int32 i = 0; i < Elements; i++)
				{
					Value[i] = Ptr[i];
				}
			}
			// BYTE
			else if (ComponentType == 5120)
			{
				int8* Ptr = (int8*)&(Bytes[Index]);
				for (int32 i = 0; i < Elements; i++)
				{
					Value[i] = bNormalized ? FMath::Max(((float)Ptr[i]) / 127.f, -1.f) : Ptr[i];
				}

			}
			// UNSIGNED_BYTE
			else if (ComponentType == 5121)
			{
				uint8* Ptr = (uint8*)&(Bytes[Index]);
				for (int32 i = 0; i < Elements; i++)
				{
					Value[i] = bNormalized ? ((float)Ptr[i]) / 255.f : Ptr[i];
				}
			}
			// SHORT
			else if (ComponentType == 5122)
			{
				int16* Ptr = (int16*)&(Bytes[Index]);
				for (int32 i = 0; i < Elements; i++)
				{
					Value[i] = bNormalized ? FMath::Max(((float)Ptr[i]) / 32767.f, -1.f) : Ptr[i];
				}
			}
			// UNSIGNED_SHORT
			else if (ComponentType == 5123)
			{
				uint16* Ptr = (uint16*)&(Bytes[Index]);
				for (int32 i = 0; i < Elements; i++)
				{
					Value[i] = bNormalized ? ((float)Ptr[i]) / 65535.f : Ptr[i];
				}
			}
			else
			{
				return false;
			}

			Data.Add(Filter(Value));
		}

		return true;
	}

	template<typename T, typename Callback>
	bool BuildFromAccessorField(TSharedRef<FJsonObject> JsonObject, const FString& Name, TArray<T>& Data, const TArray<int64>& SupportedTypes, const bool bNormalized, Callback Filter)
	{
		int64 AccessorIndex;
		if (!JsonObject->TryGetNumberField(Name, AccessorIndex))
			return false;

		TArray64<uint8> Bytes;
		int64 ComponentType, Stride, Elements, ElementSize, Count;
		if (!GetAccessor(AccessorIndex, ComponentType, Stride, Elements, ElementSize, Count, Bytes))
			return false;

		if (Elements != 1)
			return false;

		if (!SupportedTypes.Contains(ComponentType))
			return false;

		for (int64 ElementIndex = 0; ElementIndex < Count; ElementIndex++)
		{
			int64 Index = ElementIndex * Stride;
			T Value;
			// FLOAT
			if (ComponentType == 5126)
			{
				float* Ptr = (float*)&(Bytes[Index]);
				Value = *Ptr;
			}
			// BYTE
			else if (ComponentType == 5120)
			{
				int8* Ptr = (int8*)&(Bytes[Index]);
				Value = bNormalized ? FMath::Max(((float)(*Ptr)) / 127.f, -1.f) : *Ptr;

			}
			// UNSIGNED_BYTE
			else if (ComponentType == 5121)
			{
				uint8* Ptr = (uint8*)&(Bytes[Index]);
				Value = bNormalized ? ((float)(*Ptr)) / 255.f : *Ptr;

			}
			// SHORT
			else if (ComponentType == 5122)
			{
				int16* Ptr = (int16*)&(Bytes[Index]);
				Value = bNormalized ? FMath::Max(((float)(*Ptr)) / 32767.f, -1.f) : *Ptr;
			}
			// UNSIGNED_SHORT
			else if (ComponentType == 5123)
			{
				uint16* Ptr = (uint16*)&(Bytes[Index]);
				Value = bNormalized ? ((float)(*Ptr)) / 65535.f : *Ptr;
			}
			else
			{
				return false;
			}

			Data.Add(Filter(Value));
		}

		return true;
	}

	template<typename T>
	bool BuildFromAccessorField(TSharedRef<FJsonObject> JsonObject, const FString& Name, TArray<T>& Data, const TArray<int64>& SupportedElements, const TArray<int64>& SupportedTypes, const bool bNormalized)
	{
		return BuildFromAccessorField(JsonObject, Name, Data, SupportedElements, SupportedTypes, bNormalized, [&](T InValue) -> T {return InValue; });
	}

	template<typename T>
	bool BuildFromAccessorField(TSharedRef<FJsonObject> JsonObject, const FString& Name, TArray<T>& Data, const TArray<int64>& SupportedTypes, const bool bNormalized)
	{
		return BuildFromAccessorField(JsonObject, Name, Data, SupportedTypes, bNormalized, [&](T InValue) -> T {return InValue; });
	}

	template<int32 Num, typename T>
	bool GetJsonVector(const TArray<TSharedPtr<FJsonValue>>* JsonValues, T& Value)
	{
		if (JsonValues->Num() != Num)
			return false;

		for (int32 i = 0; i < Num; i++)
		{
			if (!(*JsonValues)[i]->TryGetNumber(Value[i]))
				return false;
		}

		return true;
	}

	bool MergePrimitives(TArray<FglTFRuntimePrimitive> SourcePrimitives, FglTFRuntimePrimitive& OutPrimitive);
};