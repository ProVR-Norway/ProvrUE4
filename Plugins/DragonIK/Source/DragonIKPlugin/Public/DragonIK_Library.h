/* Copyright (C) Eternal Monke Games - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Mansoor Pathiyanthra <codehawk64@gmail.com , mansoor@eternalmonke.com>, 2021
*/

#pragma once

#include "CoreMinimal.h"
//#include "Engine.h"
#include "BoneContainer.h"
#include "UObject/ObjectMacros.h"
#include "BonePose.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BoneContainer.h"

#include "UObject/NoExportTypes.h"

#include "Animation/AnimNodeBase.h"
#include "CommonAnimTypes.h"
#include "Curves/CurveFloat.h"

#include "DragonIK_Library.generated.h"






UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EIKTrace_Type_Plugin : uint8
{
	ENUM_LineTrace_Type 	UMETA(DisplayName = "Line Trace"),
	ENUM_SphereTrace_Type 	UMETA(DisplayName = "Sphere Trace"),
	ENUM_BoxTrace_Type 	UMETA(DisplayName = "Box Trace")


};



UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EInterpoLocation_Type_Plugin : uint8
{
	ENUM_DivisiveLoc_Interp 	UMETA(DisplayName = "Divisive Interpolation Method"),
	ENUM_LegacyLoc_Interp 	UMETA(DisplayName = "Normal (Location)Interpolation Method")
};



UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EInterpoRotation_Type_Plugin : uint8
{
	ENUM_DivisiveRot_Interp 	UMETA(DisplayName = "Divisive Interpolation Method"),
	ENUM_LegacyRot_Interp 	UMETA(DisplayName = "Normal (Rotation)Interpolation Method")
};





struct FDragonData_BoneStruct
{

	FBoneReference Start_Spine;
	FBoneReference Pelvis;

	FBoneReference Head_Bone;
	FBoneReference LookLimit_Bone;

	TArray<FBoneReference> FeetBones;
	TArray<FBoneReference> KneeBones;
	TArray<FBoneReference> ThighBones;

};



struct FDragonData_HitPairs
{
	FHitResult Parent_Spine_Hit;
	FVector Parent_Spine_Point;

	FHitResult Parent_Left_Hit;
	FVector Parent_Left_Point;

	FHitResult Parent_Right_Hit;
	FVector Parent_Right_Point;

	FHitResult Parent_Front_Hit;
	FVector Parent_Front_Point;
	
	FHitResult Parent_Back_Hit;
	FVector Parent_Back_Point;


	TArray<FHitResult> RV_Feet_Hits;
	TArray<FVector> RV_FeetHit_Points;


	TArray<TArray<FHitResult>> RV_Associated_Fingers_Hits;

	TArray<TArray<FHitResult>> RV_Original_Associated_Fingers_Hits;



	TArray<FHitResult> RV_FeetFront_Hits;

	TArray<FHitResult> RV_FeetBack_Hits;

	TArray<FHitResult> RV_FeetLeft_Hits;

	TArray<FHitResult> RV_FeetRight_Hits;




};



struct FDragonData_SpineFeetPair_TRANSFORM_WSPACE
{

	FTransform Spine_Involved = FTransform::Identity;

	FVector last_spine_location = FVector(0,0,0);

	FVector last_target_location = FVector(0, 0, 0);


	float Z_Offset = 0;


	TArray<FTransform> Associated_Feet = TArray<FTransform>();


	TArray<TArray<FTransform>> Associated_Fingers = TArray<TArray<FTransform>>();



	TArray<FTransform> Associated_FeetBalls = TArray<FTransform>();


	TArray<FTransform> Associated_Knee = TArray<FTransform>();


};






struct FDragonData_SpineFeetPair_heights
{

	float Spine_Involved = 0;

	TArray<float> Associated_Feet = TArray<float>();

};





/** Transient structure for FABRIK node evaluation */
struct DragonChainLink
{
public:
	/** Position of bone in component space. */
	FVector Position;

	/** Distance to its parent link. */
	float Length;

	/** Bone Index in SkeletalMesh */
	FCompactPoseBoneIndex BoneIndex;

	/** Transform Index that this control will output */
	int32 TransformIndex;

	/** Child bones which are overlapping this bone.
	* They have a zero length distance, so they will inherit this bone's transformation. */
	TArray<int32> ChildZeroLengthTransformIndices;

	DragonChainLink()
		: Position(FVector::ZeroVector)
		, Length(0.f)
		, BoneIndex(INDEX_NONE)
		, TransformIndex(INDEX_NONE)
	{
	}

	DragonChainLink(const FVector& InPosition, const float& InLength, const FCompactPoseBoneIndex& InBoneIndex, const int32& InTransformIndex)
		: Position(InPosition)
		, Length(InLength)
		, BoneIndex(InBoneIndex)
		, TransformIndex(InTransformIndex)
	{
	}
};




struct DragonSpineOutput
{
public:
	/** Position of bone in component space. */
	TArray<DragonChainLink> chain_data;
	TArray<FCompactPoseBoneIndex> BoneIndices;
	TArray<FBoneTransform> temp_transforms;
	FTransform SpineFirstEffectorTransform;
	FTransform PelvicEffectorTransform;
	FVector RootDifference;
	bool is_moved;
	int32 NumChainLinks;
};




struct FDragonData_SpineFeetPair
{

	FBoneReference Spine_Involved;

	TArray<FBoneReference> Associated_Feet;

	TArray<FBoneReference> Associated_Knees;

	TArray<FBoneReference> Associated_Thighs;

	int32 Spine_Chain_Index = 0;

	FTransform inverted_spine_tf;

	TArray<float> Feet_Heights;

//	TArray<float> feet_yaw_offset;

	TArray<FRotator> feet_rotation_offset;

	TArray<float> feet_rotation_limit;

//	TArray<float> FeetAlpha;

//	TArray<float> Max_Feet_Float;

//	TArray<float> Feet_Slope_Offset_Multiplier;

	TArray<FVector> knee_direction_offset;


	TArray<TArray<FBoneReference>> Fingers_Name_Array;

	TArray<TArray<int32>> Fingers_Chain_Num;


	TArray<int> order_index;


	TArray<FVector> feet_trace_offset;



	int solver_count = 0;

	FVector total_spine_locdata;

	bool operator == (const FDragonData_SpineFeetPair &pair) const
	{

		if (Spine_Involved.BoneIndex == pair.Spine_Involved.BoneIndex && Associated_Feet.Num() == 0)
		{
			return true;
		}
		else
			return false;


	}

};





USTRUCT(BlueprintType)
struct FDragonData_WarpLimbsData
{
	GENERATED_BODY()




	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FName Foot_Bone_Name;

	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FName Knee_Bone_Name;

	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FName Thigh_Bone_Name;


	/*
	* Controls the blue disc widgets in the viewport.
	* Place the disc just at the "floor" value of the feets/arms, which is the maximum point at which the feets/arms will go down in a walking animation.
	*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (PinHiddenByDefault))
	 float Warp_Lift_Reference_Location = 0;


	/*
	* Extra warp value offset for the limb.
	*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (PinHiddenByDefault))
		float Warp_Param_Adder = 0;



	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (DisplayName = "Minimum Maximum Warp" , PinHiddenByDefault))
		FVector2D Min_Max_Warp = FVector2D(0,2);



	/*
	* The maximum height of the extra compression of the limbs in an animation.
	* Higher values means more extra inward compression when in an animation such as walking/jogging.
	* Lower values means less arm/leg compression exaggeration in an animation.
	*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (PinHiddenByDefault))
		float max_extra_compression_height = 35;

};





struct FDragonData_WarpLimbStruct
{

		UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FBoneReference Thigh_Bone_Name;

	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FBoneReference Knee_Bone_Name;

	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FBoneReference Foot_Bone_Name;


	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (PinHiddenByDefault))
		float Warp_Lift_Reference_Location = 0;


	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (PinHiddenByDefault))
		float Warp_Param_Adder = 0;


};




USTRUCT(BlueprintType)
struct FDragonData_SingleArmElement
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Arm Target Transform", PinHiddenByDefault))
		FTransform Overrided_Arm_Transform = FTransform::Identity;


	/*
* This scales the finger line trace relative to the size of the foot trace heights.
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Alpha", PinHiddenByDefault))
		float Arm_Alpha = 1;

};


USTRUCT(BlueprintType)
struct FDragonData_Overrided_Location_Data
{
	GENERATED_BODY()

		/*
		* This array is for separate aim targets for hands.
		* It is only used if separate arm targets is enabled.
		*/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Separate Arm Target Array Override", PinHiddenByDefault))
		TArray<FDragonData_SingleArmElement> Arm_TargetLocation_Overrides;

	    
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
	//	FDragonData_Overrided_SingleArmElement test;

};

USTRUCT(BlueprintType)
struct FDragonData_ArmsData
{
	GENERATED_BODY()



		UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FBoneReference Shoulder_Bone_Name;

	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FBoneReference Elbow_Bone_Name;



	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FBoneReference Hand_Bone_Name;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FVector Local_Direction_Axis = FVector(0, 1, 0);



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FVector Arm_Aiming_Offset = FVector::ZeroVector;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
		bool accurate_hand_rotation = false;


//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
//		float overridden_limb_clamp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
		bool relative_axis = true;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
		float Maximum_Extension = 1.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
		float Minimum_Extension = 0.3f;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Elbow Pole Offset", PinHiddenByDefault))
		FVector Elbow_Pole_Offset = FVector::ZeroVector;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Overrided clamp limits ?", PinHiddenByDefault))
		bool override_limits = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Overrided (Horizontal Clamp)", PinHiddenByDefault))
		FVector2D Max_Arm_H_Angle = FVector2D(-45,45);


//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Overrided (Max Right Horizontal Clamp)", PinHiddenByDefault))
//		float Max_Arm_R_H_Angle = 45.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Overrided (Vertical Clamp)", PinHiddenByDefault))
		FVector2D Max_Arm_V_Angle = FVector2D(-45, 45);


//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Overrided (Max Down Vertical Clamp)", PinHiddenByDefault))
//		float Max_Arm_D_V_Angle = 45.0f;


};



USTRUCT(BlueprintType)
struct FDragonData_FingerData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
	 FName Finger_Bone_Name;



	/*
* This scales the finger line trace relative to the size of the foot trace heights.
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Trace size scale", PinHiddenByDefault))
		float Trace_Scale = 0.5f;


	/*
* Tweak this to offset the trace lines from its original calculated position.
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FVector Trace_Offset;


/*
* If finger is pointing backwards relative to the forward vector, enable this.
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Is finger pointing backwards ?", PinHiddenByDefault))
		bool Is_Finger_Backward = false;


//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
	 float chain_number = 2;



};

USTRUCT(BlueprintType)
struct FDragonData_FootData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FName Feet_Bone_Name;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FName Knee_Bone_Name;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FName Thigh_Bone_Name;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FRotator Feet_Rotation_Offset;



	/*
	* If fixed pole is disabled, then pole is calculated related to the knee animations.
	* If fixed pole is enabled, then pole is calculated strictly in component space fixed locations.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Fixed Pole ?",PinHiddenByDefault))
		bool Fixed_Pole = false;

	/*
	* Tweak this to control the knee poles. Visually controls the blue ball widgets in the graph viewport.
	* If fixed pole is disabled, then location is relative to the knee bones.
	* If fixed pole is enabled, then location is relative to component space of the skeletal mesh.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Knee Pole Offset",PinHiddenByDefault))
		FVector Knee_Direction_Offset = FVector(0,0,0);





	/*
	* Tweak this to offset the trace lines from its original calculated position.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FVector Feet_Trace_Offset = FVector(0, 0, 0);


	/*
	* This is only used in the foot solver and if 4-point feet rotation is enabled
	* This tweaks the length of the feet.
	* Visualize the size using the animbp viewport widgets.
	* The length of the yellow cylinder widget on the feet.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Frontal spacing (4-point feets)",PinHiddenByDefault))
		float Front_Trace_Point_Spacing = 20;


	/*
* This is only used in the foot solver and if 4-point feet rotation is enabled
* This tweaks the width of the feet.
* Visualize the width using the animbp viewport widgets.
* The yellow cylindrical widget's radius denotes the width.
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Side spacing (4-point feets)", PinHiddenByDefault))
		float Side_Traces_Spacing = 5;

	
	/*
	* Maximum foot rotation in all axes
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
		float Feet_Rotation_Limit = 45;


	/*
	* Use constant fixed height for feet instead of automatically calculated depending on animation.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Fixed Foot Height ?"))
		bool Fixed_Foot_Height = false;

	/*
	* If fixed foot height is disabed, this will provide an offset to the automatic feet height.
	* If fixed foot height is disabled, this will set the fixed feet height.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Feet Height Offset / Fixed Feet Height"))
		float Feet_Heights;



	/*
	* The influence of IK.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
		float Feet_Alpha = 1;

	/*
	* Minimum feet extension percent.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Minimum Feet extension ratio", PinHiddenByDefault))
		float Min_Feet_Extension = 0.35f;


	/*
	* Maximum feet extension percent.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Maximum Feet extension ratio", PinHiddenByDefault))
		float Max_Feet_Extension = 1.25f;


	/*
	* Lift/Dip the feet as it rotates on slopes.
	* Use small values like 0.1f, because this is a multiplier which increases/decreases the lift depending on the pitch/roll difference of the feet as it touches steep slopes.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Feet Slope Offset Multiplier", PinHiddenByDefault))
		float Feet_Slope_Offset_Multiplier = 0.0f;




	/*
* if value is above 0, the trace radius is overrided.
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Overrided trace radius (if capsule trace used)", PinHiddenByDefault))
		float Overrided_Trace_Radius = 0.0f;




	/*
	* Finger arrays. Include just the first bone in each finger chain.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Finger Array (optional)",  PinHiddenByDefault))
		TArray<FDragonData_FingerData> Finger_Array;

};


USTRUCT(Blueprintable)
struct FDragonData_MultiInput
{
	GENERATED_USTRUCT_BODY()

public:

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (DisplayName = "Start Spine / Chest",PinHiddenByDefault))
		FName Start_Spine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
		FName Pelvis;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinHiddenByDefault))
		TArray<FDragonData_FootData> FeetBones;

};


///////////







USTRUCT()
struct FCCDIK_Modified_ChainLink
{
	GENERATED_BODY()
public:
	/** Position of bone in component space. */
	FVector Position;

	/** Local Solver Positions */
	FVector solverLocalPositions;

	/** Rotation of bone in component space. */
	FQuat BoneRotation;

	/** Distance to its parent link. */
	float Length;

	/** Axis of rotation **/
	FVector axis;

	/** Bone Index in SkeletalMesh */
	int32 BoneIndex;

	/** Transform Index that this control will output */
	int32 TransformIndex;

	/** Default Direction to Parent */
	FVector DefaultDirToParent;

	/** Child bones which are overlapping this bone.
	* They have a zero length distance, so they will inherit this bone's transformation. */
	TArray<int32> ChildZeroLengthTransformIndices;

	FCCDIK_Modified_ChainLink()
		: Position(FVector::ZeroVector)
		, Length(0.f)
		, BoneIndex(INDEX_NONE)
		, TransformIndex(INDEX_NONE)
		, DefaultDirToParent(FVector(-1.f, 0.f, 0.f))
	{
	}

	FCCDIK_Modified_ChainLink(const FVector& InPosition, const FVector& LocalPosition, const FQuat& InRotation, const float InLength, const FCompactPoseBoneIndex& InBoneIndex, const int32& InTransformIndex)
		: Position(InPosition)
		, solverLocalPositions(LocalPosition)
		, BoneRotation(InRotation)
		, Length(InLength)
		, BoneIndex(InBoneIndex.GetInt())
		, TransformIndex(InTransformIndex)
		, DefaultDirToParent(FVector(-1.f, 0.f, 0.f))
	{
	}

	FCCDIK_Modified_ChainLink(const FVector& InPosition, const float InLength, const FCompactPoseBoneIndex& InBoneIndex, const int32& InTransformIndex, const FVector& InDefaultDirToParent)
		: Position(InPosition)
		, Length(InLength)
		, BoneIndex(InBoneIndex.GetInt())
		, TransformIndex(InTransformIndex)
		, DefaultDirToParent(InDefaultDirToParent)
	{
	}

	FCCDIK_Modified_ChainLink(const FVector& InPosition, const float InLength, const int32 InBoneIndex, const int32 InTransformIndex)
		: Position(InPosition)
		, Length(InLength)
		, BoneIndex(InBoneIndex)
		, TransformIndex(InTransformIndex)
		, DefaultDirToParent(FVector(-1.f, 0.f, 0.f))
	{
	}
};






void Solve_Modified_TwoBoneIK(const FVector& RootPos, const FVector& JointPos, const FVector& EndPos, const FVector& JointTarget, const FVector& Effector, const FVector& ThighEffector, FVector& OutJointPos, FVector& OutEndPos, float UpperLimbLength, float LowerLimbLength, bool bAllowStretching, float StartStretchRatio, float MaxStretchScale);
void Solve_Modified_TwoBoneIK_2(const FVector& RootPos, const FVector& JointPos, const FVector& EndPos, const FVector& JointTarget, const FVector& Effector, const FVector& ThighEffector, FVector& OutJointPos, FVector& OutEndPos, bool bAllowStretching, float StartStretchRatio, float MaxStretchScale);
void Solve_Modified_TwoBoneIK_3(FTransform& InOutRootTransform, FTransform& InOutJointTransform, FTransform& InOutEndTransform, const FVector& JointTarget, const FVector& Effector, const FVector& ThighEffector, float UpperLimbLength, float LowerLimbLength, bool bAllowStretching, float StartStretchRatio, float MaxStretchScale);
void Solve_Modified_TwoBoneIK_4(FTransform& InOutRootTransform, FTransform& InOutJointTransform, FTransform& InOutEndTransform, const FVector& JointTarget, const FVector& Effector, const FVector& ThighEffector, bool bAllowStretching, float StartStretchRatio, float MaxStretchScale);





void Solve_Modified_Direct_TwoBoneIK(const FVector& RootPos, const FVector& JointPos, const FVector& EndPos, const FVector& JointTarget, const FVector& Effector, FVector& OutJointPos, FVector& OutEndPos, float UpperLimbLength, float LowerLimbLength, bool bAllowStretching, float StartStretchRatio, float MaxStretchScale);
void Solve_Modified_Direct_TwoBoneIK_2(const FVector& RootPos, const FVector& JointPos, const FVector& EndPos, const FVector& JointTarget, const FVector& Effector, FVector& OutJointPos, FVector& OutEndPos, bool bAllowStretching, float StartStretchRatio, float MaxStretchScale);
void Solve_Modified_Direct_TwoBoneIK_3(FTransform& InOutRootTransform, FTransform& InOutJointTransform, FTransform& InOutEndTransform, const FVector& JointTarget, const FVector& Effector, float UpperLimbLength, float LowerLimbLength, bool bAllowStretching, float StartStretchRatio, float MaxStretchScale);
void Solve_Modified_Direct_TwoBoneIK_4(FTransform& InOutRootTransform, FTransform& InOutJointTransform, FTransform& InOutEndTransform, const FVector& JointTarget, const FVector& Effector, bool bAllowStretching, float StartStretchRatio, float MaxStretchScale);




UCLASS(Blueprintable, BlueprintType)
class DRAGONIKPLUGIN_API UDragonIK_Library : public UObject
{
	GENERATED_BODY()



		static UDragonIK_Library* Constructor();


public :

	UFUNCTION(BlueprintPure, Category = "SolverFunctions")
		static FRotator CustomLookRotation(FVector lookAt, FVector upDirection);

	static FQuat UnityLookRotation(FVector forward, FVector upward);


	static FQuat UnityRotateTowards(FQuat from, FQuat to, float maxDegreesDelta);

	static FQuat SafeFromToRotation(FVector v1, FVector v2, FVector up);

	static FQuat QuatFromToRotation(FQuat from, FQuat to);


	//FABRIK RELEVANT FUNCTIONS

	 static FVector SolveJoint(FVector pos1, FVector pos2, float length);

	 static void SolverMove(TArray<FCCDIK_Modified_ChainLink>& InOutChain, int index, FVector offset);

	 static void SolverRotate(TArray<FCCDIK_Modified_ChainLink>& InOutChain, int index, FQuat rotation, bool recursive);

	 static void SolverRotateChildren(TArray<FCCDIK_Modified_ChainLink>& InOutChain, int index, FQuat rotation);

	 static void SolverMoveChildrenAroundPoint(TArray<FCCDIK_Modified_ChainLink>& InOutChain, int index, FQuat rotation);

	 static FQuat GetParentSolverRotation(TArray<FCCDIK_Modified_ChainLink>& InOutChain, int index);

	 static FVector GetParentSolverPosition(TArray<FCCDIK_Modified_ChainLink>& InOutChain, int index);

	 static FQuat GetLimitedRotation(TArray<FCCDIK_Modified_ChainLink>& InOutChain, FQuat OriginalBoneRotation, int index, FQuat q, bool &changed);
	 static FQuat GetLimitedLocalRotation(FQuat defaultLocalRotation, FQuat localRotation, bool& changed);

	 static FQuat LimitTwist(FQuat rotation, FVector axis, FVector orthoAxis, float twistLimit);
	 static FQuat LimitRotation(FQuat rotation, FVector axis, FVector secondaryAxis, float general_limit, float twistLimit);
	 static FQuat LimitSwing(FQuat rotation, FVector axis, float general_limit);

	 static void LimitForward(int rotateBone, int limitBone, TArray<FCCDIK_Modified_ChainLink>& InOutChain);

	 static void BackwardReachLimited(FVector position, TArray<FCCDIK_Modified_ChainLink>& InOutChain);

	//

	UFUNCTION(BlueprintPure, Category = "SolverFunctions")
		static FRotator LookAtVector_V2(FVector Source_Location, FVector lookAt, FVector upDirection);



	static FTransform QuatLookXatLocalDirection(const FTransform& LookAtFromTransform, const FVector& LookAtLocalDirection);

	UFUNCTION(BlueprintPure, Category = "SolverFunctions")
		static FTransform QuatLookXatLocation(const FTransform& LookAtFromTransform, const FVector& LookAtTarget);

	UFUNCTION(BlueprintPure, Category = "SolverFunctions")
	static FRotator LookAtRotation_V3(FVector source, FVector target,FVector upvector);



	static FRotator LookAt_Function(FTransform transform, FVector worldPosition, FVector up_direction, FVector rotationOffset);


	static bool Solve_Modified_CCDIK(TArray<FCCDIK_Modified_ChainLink>& InOutChain, const FVector& TargetPosition, float MaximumReach, float Precision, int32 MaxIteration);

	

	//FTransform GetTargetTransform_Modified(const FTransform& InComponentTransform, FCSPose<FCompactPose>& MeshBases, FBoneSocketTarget& InTarget, const FVector& InOffset);


	static void Evaluate_CCDIK_Modified(bool First_Start, FComponentSpacePoseContext& Output, USkeletalMeshComponent *owning_skel, FBoneReference RootBone, FBoneReference TipBone, float Precision, int32 MaxIteration, bool bStartFromTail, bool bEnableRotationLimit, FTransform EffectorTransform, TArray<FBoneTransform>& OutBoneTransforms, TArray<FBoneTransform>& Dulicate_OutBoneTransforms);

	static void Evaluate_ConsecutiveBoneRotations(FComponentSpacePoseContext& Output, USkeletalMeshComponent* owning_skel,TArray<FDragonData_SpineFeetPair> spine_feet_pair, FRuntimeFloatCurve Look_Bending_Curve, FBoneReference RootBone_Input, FBoneReference TipBone_Input, float Lookat_Radius, FTransform EffectorTransform, FAxis LookAt_Axis, float Lookat_Clamp,float Vertical_Dip_Treshold,float Downward_Dip_Multiplier,float Inverted_Dip_Multiplier, float Side_Multiplier,float Side_Down_Multiplier,bool alter_pelvis,FTransform PelvisLocTarget, FRuntimeFloatCurve Bending_Multiplier_Curve,float Up_Rot_Clamp,bool Use_Natural_Method, bool separate_head_clamp, float head_clamp_value,FTransform HeadTransf, TArray<FBoneTransform>& OutBoneTransforms);

	bool Lock_Forward_Axis = true;




//	static void Solve_Modified_TwoBoneIK(const FVector& RootPos, const FVector& JointPos, const FVector& EndPos, const FVector& JointTarget, const FVector& Effector, FVector& OutJointPos, FVector& OutEndPos, float UpperLimbLength, float LowerLimbLength, bool bAllowStretching, float StartStretchRatio, float MaxStretchScale);
	static void Evaluate_TwoBoneIK_Modified(FComponentSpacePoseContext& Output, USkeletalMeshComponent* owning_skel, FBoneReference FeetBone, FBoneReference KneeBone, FBoneReference ThighBone, FTransform ThighTransform, FVector JointLocation, FVector Knee_Pole_Offset, TArray<FBoneTransform>& OutBoneTransforms);

	static void Evaluate_TwoBoneIK_Direct_Modified(FComponentSpacePoseContext& Output, USkeletalMeshComponent* owning_skel, FBoneReference FeetBone, FBoneReference KneeBone, FBoneReference ThighBone, FTransform ThighTransform , FTransform Shoulder,FTransform Knee,FTransform Hand, FVector JointLocation, FVector Knee_Pole_Offset, FTransform transform_offset , FTransform Common_Spine_Modified_Transform, FRotator Limb_Rotation_Offset, FVector HandAxis, bool is_relative_axis,float Hand_Clamp_Value,bool accurate_hand_rotation, FTransform Extra_Hand_Offset,FVector Elbow_Pole_Offset , bool override_hand_rotation,FTransform Knee_Transform_Default, TArray<FBoneTransform>& OutBoneTransforms);

	static void Evaluate_TwoBoneIK_Spinal_Modified(FComponentSpacePoseContext& Output, USkeletalMeshComponent* owning_skel, FBoneReference FeetBone, FBoneReference KneeBone, FBoneReference ThighBone, FTransform ThighTransform, FTransform Shoulder_Trans, FTransform Knee_Trans, FTransform Hand_Trans, FVector JointLocation, FVector Knee_Pole_Offset, TArray<FBoneTransform>& OutBoneTransforms);

	static FTransform LookAt_Processor(FTransform ComponentBoneTransform, FVector HeadLocation, FVector Offset_Vector, FAxis LookAt_Axis, float Lookat_Clamp, bool Use_Natural_Method, float Up_Rotation_Clamp, float intensity);

	static FVector LookAt_Fabrik_Processor(FTransform ComponentBoneTransform, FVector Offset_Vector, FVector LookAt_Axis, float Lookat_Clamp);

	static void LimitFabrikBoneRotations(TArray<FCCDIK_Modified_ChainLink>& InOutChain);


	static void OrthoNormalize(FVector& Normal, FVector& Tangent);

};





/**
 * 

UCLASS()
class DRAGONIKPLUGIN_API UDragonIK_Library : public UObject
{
	GENERATED_BODY()
	
	
	
	
};
*/