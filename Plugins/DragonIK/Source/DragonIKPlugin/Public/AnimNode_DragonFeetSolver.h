/* Copyright (C) Eternal Monke Games - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Mansoor Pathiyanthra <codehawk64@gmail.com , mansoor@eternalmonke.com>, 2021
*/

#pragma once



#include "DragonIK_Library.h"
#include "CoreMinimal.h"

#include "DragonIKPlugin.h"

#include "Kismet/KismetMathLibrary.h"

#include "CollisionQueryParams.h"

#include "Animation/InputScaleBias.h"
#include "AnimNode_DragonControlBase.h"
#include "AnimNode_DragonFeetSolver.generated.h"
/**
 * 
 */


class USkeletalMeshComponent;




UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EIK_Type_Plugin : uint8
{
	ENUM_Two_Bone_Ik 	UMETA(DisplayName = "Two Bone IK"),
	ENUM_Single_Bone_Ik 	UMETA(DisplayName = "Single Bone IK")

};



//USTRUCT(BlueprintInternalUseOnly)

USTRUCT(BlueprintType)
struct DRAGONIKPLUGIN_API FAnimNode_DragonFeetSolver : public FAnimNode_DragonControlBase
{
	//	GENERATED_USTRUCT_BODY()
	GENERATED_BODY()

public:



	
		/*
		* Type the input bones used by the solver - pelvis,spine-start and feets
  * Pull this pin to create a "make struct" and easily type out the bones.
  * Optionally you can type the bones directly inside the solver. Doing so is slightly more recommended because it activates fast path.
  */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputData, meta = (PinHiddenByDefault))
		FDragonData_MultiInput dragon_input_data;

	FDragonData_BoneStruct dragon_bone_data;





	FAnimNode_DragonFeetSolver();

	int test_counter = 0;

	int trace_draw_counter = 0;


	/*
	* Select foot ik type - two bone ik and one bone ik.
	* 99.9% best to use the default two bone ik.
	* One bone ik is only useful in case the animal has no knee bones, such as the infinity blade spiders.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TraceSettings)
	 EIK_Type_Plugin ik_type = EIK_Type_Plugin::ENUM_Two_Bone_Ik;


	/*
	* Choose Trace type - Line,Sphere and Box.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TraceSettings, meta = (DisplayName = "Trace Type"))
	 EIKTrace_Type_Plugin trace_type = EIKTrace_Type_Plugin::ENUM_LineTrace_Type;




	/*
	* If trace type is box or sphere, its radius is controlled using the Trace Radius */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TraceSettings, meta = (DisplayName = "Trace Radius", PinHiddenByDefault))
		float Trace_Radius = 20;




	/*
* If enabled, the velocity that drives the curves is the custom velocity parameter instead of calculating from the character blueprint movement component.
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MasterCurveSettings, meta = (DisplayName = "Use custom velocity instead of automatic character blueprint velocity for curves ?", PinHiddenByDefault))
		bool Override_Curve_Velocity = false;


	/*
	* Used as the velocity for curves if enabled.
	* Overrides the automatic velocity calculation, useful for non-Character blueprints that doesn't have a movement component.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MasterCurveSettings, meta = (DisplayName = "Custom Curve Velocity", PinHiddenByDefault))
		float custom_velocity = 0;



	/*
	* Select the feet location interpolation method.
	* Default uses the divisive location interpolation method, which provides optimal smoothness and speed of solving.
	* Optionally can use the legacy method of interpolation.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InterpolationSettings, meta = (DisplayName = "Location Interpolation Type"))
		EInterpoLocation_Type_Plugin loc_interp_type = EInterpoLocation_Type_Plugin::ENUM_LegacyLoc_Interp;



	float scale_mode = 1;

	/*
	* Select the feet rotation interpolation method.
	* Default uses the divisive location interpolation method, which provides optimal smoothness and speed of solving.
	* Optionally can use the legacy method of interpolation.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InterpolationSettings, meta = (DisplayName = "Rotation Interpolation Type"))
		EInterpoRotation_Type_Plugin rot_interp_type = EInterpoRotation_Type_Plugin::ENUM_LegacyRot_Interp;



	/*
* Virtual scale multiplier of the ik
* Increase this value if your mesh is super big by default and you are too lazy to increase all parameters one by one.
* Uniformly increases all trace related parameters.
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BasicSettings, meta = (DisplayName = "Global trace scale multiplier", PinHiddenByDefault))
		float virtual_scale = 1;



//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
//
//	FComponentSpacePoseLink ComponentPose;

	/*
* Alpha of the entire solver. 0 means no solving and 1 means maximum solving.
*/
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (PinShownByDefault))
//		mutable float Alpha = 1;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
//		FInputScaleBias AlphaScaleBias;




	/*
	* Parameter to choose between automatic feet bone detection or manual method.
	* If enabled, solvers only uses the feet bones, and automatically assumes the next 2 parent bones as knees and thighs.
	* If disabled, solvers uses the feet bones, knee bones and thigh bones typed in the feet array.
	* If disabled, all bones need to be valid. Any invalid bones will not activate the ik.
	* Very useful to keep it disabled on DAZ rigs and certain animal characters, where the thigh-knee-foot are not in a straight linear hierarchy.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Automatic Foot-Knee-Thigh detection", PinHiddenByDefault))
		bool automatic_leg_make = true;


	/*
	* If enabled, the rotation of the feet will be relative to its current animation.
	* If disabled, the absolute reference rotation of the feet will be used instead, ensuring always aligned feets.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Use (optional ref pose) as reference for foot rotation ?", PinHiddenByDefault))
		bool Use_OptionalRef_Feet_As_Ref = true;


	//bool Use_Animated_Feet_As_Ref = false;


	/*
* Toggle this parameter to turn on/off ik.
* Example use case : Disable it when character is jumping or flying in the air.
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Enable Solver", PinShownByDefault))
		bool enable_solver = true;


	/*
* Enable this parameter to make IK work even when not playing in PIE mode
* Useful for recording in sequencer.
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AdvancedSettings, meta = (DisplayName = "Work outside gameplay (For Sequencer)", PinShownByDefault))
		bool Work_Outside_PIE = false;



	/*
	* Supply a cached pose copy before the spine solver to serve as a proper reference for calculating feet rotations on slopes.
	* Only used if its boolean is enabled.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputData, meta = (DisplayName = "(optional) Ref pose for foot rotation on slopes"))
		FComponentSpacePoseLink OptionalRefPose;





	/*
	* If enabled, interpolation is strictly on the vertical axis of the feets. (Recommended)
	* If disabled, interpolation is in all directions. Might create a delayed feet response in low frame rates.
	*/
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InterpolationSettings, meta = (DisplayName = "Interpolate only on legs Z axis ?", PinHiddenByDefault))
		bool interpolate_only_z = false;



	/*
 * The transistion speed between solve and unsolve state of the leg ik (eg:- when character jumps and falls back to ground).
 * Lower values means slower but smoother transition.
 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InterpolationSettings, meta = (DisplayName = "Shift Speed", PinHiddenByDefault))
		float shift_speed = 2;



	float Delta_Loc_Speed = 0;

	float Delta_Rot_Speed = 0;






	/*
* Controls the feet interpolation location speed.
* Lower values means smoother but slower results.
* Is ignored if "Ignore location lerping" is enabled.
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InterpolationSettings, meta = (DisplayName = "Feet Position Interpolation Speed", PinHiddenByDefault))
		float Location_Lerp_Speed = 1;


	/*
* Controls the feet interpolation rotation speed.
* Lower values means smoother but slower results.
* Is ignored if "Ignore rotation lerping" is enabled.
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InterpolationSettings, meta = (DisplayName = "Feet Rotation Interpolation Speed", PinHiddenByDefault))
		float feet_rotation_speed = 2;




	/*
* Shift Logic : The smooth transition between touching and non-touching state.
* Ignoring this will make transitions instant.
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InterpolationSettings, meta = (DisplayName = "Ignore Shift Logic", PinHiddenByDefault))
		bool ignore_shift_speed = false;



	/*
	* Enable this to completely bypass rotation interpolation and use default values!
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InterpolationSettings, meta = (DisplayName = "Ignore Rotation Interpolation", PinHiddenByDefault))
		bool Ignore_Lerping = false;


	/*
	* Enable this to completely bypass location interpolation and use default values!
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InterpolationSettings, meta = (DisplayName = "Ignore Location Interpolation", PinHiddenByDefault))
		bool Ignore_Location_Lerping = false;




	UPROPERTY(EditAnywhere, Category = InterpolationSettings, meta = (DisplayName = "Interpolation multiplier relative to velocity", PinHiddenByDefault))
		FRuntimeFloatCurve Interpolation_Velocity_Curve;


	/*
	* Uses a rotation method of rotating and moving the feets to perfectly align themselves to the terrain.
	* If disabled, only the feets get rotated. Might produce slight gaps on steep terrains.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InterpolationSettings, meta = (DisplayName = "Enable complex but accurate foot placement method ?", PinHiddenByDefault))
		bool Enable_Complex_Rotation_Method = false;


	UPROPERTY(EditAnywhere, Category = InterpolationSettings, meta = (DisplayName = "Complex to simple foot placement transition relative to velocity", PinHiddenByDefault))
		FRuntimeFloatCurve ComplexSimpleFoot_Velocity_Curve;



	TArray<FVector> TraceStartList = TArray<FVector>();
	TArray<FVector> TraceEndList = TArray<FVector>();
	TArray<bool> Is_Line_Mode = TArray<bool>();
	TArray<float> TraceRadiusList = TArray<float>();

	virtual void ConditionalDebugDraw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* PreviewSkelMeshComp) const;

	/*
* Trace channel used by the solver traces. Recommended to create a new dedicated trace channel for the ik through project settings.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TraceSettings)
		TEnumAsByte<ETraceTypeQuery> Trace_Channel = ETraceTypeQuery::TraceTypeQuery1;



		/*Channel used for the trace repelling anti-channel process.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TraceSettings, meta = (DisplayName = "Trace Anti-Channel"))
		TEnumAsByte<ETraceTypeQuery> Anti_Trace_Channel = ETraceTypeQuery::TraceTypeQuery2;


	/*
* Max LOD that this node is allowed to run
* For example if you have LODThreadhold to be 2, it will run until LOD 2 (based on 0 index)
* when the component LOD becomes 3, it will stop update/evaluate
*/

//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Performance, meta = (DisplayName = "LOD Threshold"))
//		int32 LODThreshold = -1;




	virtual int32 GetLODThreshold() const override { return LODThreshold; }


	
	/*
	* Disable interpolation of the foot solver if the game frame rate falls beyond this value.
	* Useful to avoid the flying and inconsistent feet problem at lower frame rates.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TraceSettings, meta = (DisplayName = "Fps treshold for interpolation"))
		float FPS_Lerp_Treshold = 25;

	float current_fps = -1;


//	UPROPERTY(Transient)
//		float ActualAlpha = 0;


//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EndEffector, meta = (PinShownByDefault))
//		FTransform DebugEffectorTransform;


	FBoneContainer* SavedBoneContainer;


	FTransform ChestEffectorTransform = FTransform::Identity;

	FTransform RootEffectorTransform = FTransform::Identity;


	TArray<FBoneReference> feet_bone_array;

	TArray<FTransform> feet_transform_array;


	TArray<TArray<float>> feet_Alpha_array;


	TArray<TArray<FTransform>> feet_mod_transform_array;

	TArray<TArray<FVector>> feet_modified_normals;


	TArray<TArray<bool>> feet_ishit_array;
	TArray<TArray<FVector>> feet_impactpoint_array;


	TArray<TArray<TArray<FTransform>>> feet_fingers_transform_array;



	TArray<TArray<FVector>> feet_knee_offset_array;


	TArray<TArray<FTransform>> feet_Animated_transform_array;
	TArray<FTransform> knee_Animated_transform_array;



	bool atleast_one_hit = false;

	TArray<FHitResult> feet_hit_array;

	bool solve_should_fail = false;





	TArray<FDragonData_SpineFeetPair> spine_Feet_pair;
	TArray<FName> Total_spine_bones;

	bool every_foot_dont_have_child = false;

	FName GetChildBone(FName BoneName, USkeletalMeshComponent* skel_mesh);


	void Leg_ik_Function(FBoneReference ik_footbone, int spine_index, int feet_index, TEnumAsByte<enum EBoneControlSpace> EffectorLocationSpace, TEnumAsByte<enum EBoneControlSpace> JointTargetLocationSpace, FComponentSpacePoseContext& MeshBasesSaved, TArray<FBoneTransform>& OutBoneTransforms);



	void Leg_Singleik_Function(FBoneReference ik_footbone, int spine_index, int feet_index, TEnumAsByte<enum EBoneControlSpace> EffectorLocationSpace, TEnumAsByte<enum EBoneControlSpace> JointTargetLocationSpace, FComponentSpacePoseContext& MeshBasesSaved, TArray<FBoneTransform>& OutBoneTransforms);




	void Leg_Full_Function(FName foot_name, int spine_index, int feet_index, FComponentSpacePoseContext& MeshBasesSaved, TArray<FBoneTransform>& OutBoneTransforms);

	TArray<FName> BoneArrayMachine(int32 index, FName starting, FName ending, bool is_foot = false);


	TArray<FName> BoneArrayMachine_Feet(int32 index, FName starting,FName knee,FName thigh, FName ending, bool is_foot = false);



	bool Check_Loop_Exist(int order_index,float Feet_Slope_Offset_Multiplier, TArray<FDragonData_FingerData> Finger_Array, float FeetAlpha, float MaxFleetFloat,FVector Feet_Trace_Offset,FVector knee_direction_offset,float feet_rotation_limit,FRotator feet_rotation_offset,float feet_height, FName start_bone, FName knee_bone, FName thigh_bone, FName input_bone, TArray<FName> total_spine_bones);

	TArray<FDragonData_SpineFeetPair> Swap_Spine_Pairs(TArray<FDragonData_SpineFeetPair> test_list);


	FVector AnimLocLerp(bool is_Hit, int spine_index, int feet_index, FVector start_pos, FVector end_pos,float delta_seconds);


	FRotator AnimRotLerp(bool is_Hit, int spine_index, int feet_index, FRotator start_pos, FRotator end_pos, float delta_seconds);

	FQuat AnimRotSLerp(bool is_Hit, int spine_index, int feet_index, FQuat start_pos, FQuat end_pos, float delta_seconds);




	FRotator RotationFromImpactNormal(FComponentSpacePoseContext& Output, FVector normal_impact, FTransform original_Bone_transform,float feet_limit);



//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "LineTraceHeight", PinHiddenByDefault))
//		float line_trace_height = 1000;


	/*
	* Line trace height above the feet bones.
	* Too high values will cause legs to react to ceilings and trees.
	* Too low values will cause ik to not work on extreme slopes and steps.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TraceSettings, meta = (DisplayName = "Trace Height above feet", PinHiddenByDefault ))
		float line_trace_upper_height = 200;


	/*
* Line trace height below the feet bones. Usually best kept 0.
* Too high values can lead to sticky IK which might be undesirable.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TraceSettings, meta = (DisplayName = "Trace Height below feet", PinHiddenByDefault ))
		float line_trace_down_height = 5;



/*
* 
*/
	UPROPERTY(EditAnywhere, Category = TraceSettings, meta = (DisplayName = "Trace down height multiplier relative to velocity", PinHiddenByDefault))
		FRuntimeFloatCurve Trace_Down_Multiplier_Curve;




	/*
	* Use the anti-channel in the solving logic. Use meshes with the anti-channel set to "block" to repel the traces from touching ceilings and closed spaces.
	* Also useful when under stairs or narrow multi-storied buildings. Cover the ceilings and under stairs with anti-channel blocked meshes.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TraceSettings, meta = (DisplayName = "Use Anti-Channel Functionality", PinHiddenByDefault ))
		bool Use_Anti_Channel = false;














//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Rotation_Lerp_Speed", PinHiddenByDefault))
//		float Rotation_Lerp_Speed = 25;


	/*
	* Disable this to ignore feet rotaion and use default animated rotation.
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Should Solving Rotate Feet ?", PinHiddenByDefault))
		bool Should_Rotate_Feet = true;



	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Experimental, meta = (DisplayName = "Use_Feet_Tips(If Exist) ?", PinHiddenByDefault))
		bool Use_Feet_Tips = false;


		/*
* Enable this to render the line traces in the animgraph viewport.
*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Display Line Trace ?", PinHiddenByDefault))
		bool show_trace_in_game = true;


//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Automatic Foot Height Detection", PinHiddenByDefault))
		bool Automatic_Foot_Height_Detection = true;


		float Character_Speed = 0;

		AActor* Character_Actor;




	bool is_initialized = false;

	bool first_time_setup = true;

	int first_time_count = 0;

	/*
	* Disable this to zero out the pitch of the feet.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Enable Foot Pitch", PinHiddenByDefault))
		bool Enable_Pitch = true;







	/*
	* Disable this to zero out the roll of the feet.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Enable Foot Roll", PinHiddenByDefault))
		bool Enable_Roll = true;



	/*
*The up direction vector of the character in component space.
* 99% cases, this should not be altered.
* Only needed to alter on characters that do not follow the standard unreal character orientations.
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Character Up Direction Vector (Local space)", PinHiddenByDefault))
		FVector character_direction_vector_CS = FVector(0,0,1);




	/*
*The up direction vector of the character in component space.
* 99% cases, this should not be altered.
* Only needed to alter on characters that do not follow the standard unreal character orientations.
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Character Forward Direction Vector (Local space)", PinHiddenByDefault))
		FVector character_forward_direction_vector_CS = FVector(0, 1, 0);



	/*
	* Disabling this uses the default foot rotation calculation, using the terrain normal data.
	* Enabling this uses a slightly more complex but more stable and accurate rotation calculation. Uses 4 traces per feet instead of 1.
	* Provides greater control of rotation control in the foot array. Have the ability to control the frontal and sideway spacing per feet.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Use advanced 4-point feet rotation ?", PinHiddenByDefault))
		bool Use_Four_Point_Feets = false;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Enable foot lift limit ? (uses the foot extension ratios in foot array)", PinHiddenByDefault))
		bool Enable_Foot_Lift_Limit = true;


	/*
	* Disable this to completely disable finger ik if finger data is written.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Calculate toe/finger rotations ?", PinHiddenByDefault))
		bool Affect_Toes_Always = true;


	/*
	* This will gradually turn off finger ik when in locomotion to give smoother and stable results during gameplay.
	* Default values make finger ik only available when idle. Tweak it to enable it during gameplay.
	* 
	*/
	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayName = "Finger IK Alpha relative to velocity", PinHiddenByDefault))
		FRuntimeFloatCurve Finger_Alpha_Velocity_Curve;


	/*
	* Decrease this to clamp the leg ik within the angle.
	* Can help decrease jarring knee bending on extreme terrain conditions.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Maximum allowed leg ik angle", PinHiddenByDefault))
		float Max_Limb_Radius = 90;



//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpeedWarpSettings, meta = (DisplayName = "Speed warping constant", PinHiddenByDefault))
//		float speed_warping_const = 1;



//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Use Ref Pose Rotation as reference", PinHiddenByDefault))
//		bool use_ref_rotation = false;

	

//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (DisplayName = "Maximum Feet-Terrain Distance", PinHiddenByDefault))
//		float Vertical_Solver_downward_Scale = 250;




//		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ComponentDirectionSettings, meta = (DisplayName = "Flip forward and right rotation", PinHiddenByDefault))
//			bool flip_forward_and_right = false;


	TArray<FVector> EffectorLocationList;


	TArray<float> Total_spine_heights;
	TArray<FDragonData_HitPairs> spine_hit_pairs;

	TArray<FCompactPoseBoneIndex> Spine_Indices;





	TArray<FDragonData_SpineFeetPair_TRANSFORM_WSPACE> spine_Transform_pairs;
	TArray<FDragonData_SpineFeetPair_TRANSFORM_WSPACE> spine_AnimatedTransform_pairs;


	TArray<TArray<FVector>> FeetTipLocations;
	TArray<TArray<float>> FeetWidthSpacing;



	TArray<TArray<float>> FeetRootHeights;

	TArray<TArray<TArray<float>>> FeetFingerHeights;



	void GetFeetHeights(FComponentSpacePoseContext & Output);



	void Calculate_Feet_Rotation(FComponentSpacePoseContext & Output, TArray<TArray<FTransform>> feet_rotation_array);




	FCollisionQueryParams getDefaultColliParams(FName name, AActor *me);

	void line_trace_func(USkeletalMeshComponent *skelmesh, FVector startpoint, FVector endpoint, FHitResult RV_Ragdoll_Hit, FName bone_text, FName trace_tag, float trace_radius_val, FHitResult& Output, const FLinearColor& Fdebug_color,bool render_trace,bool is_line);


	FComponentSpacePoseContext* saved_pose;

	USkeletalMeshComponent *owning_skel;

	TArray<FBoneTransform> RestBoneTransforms;
	TArray<FBoneTransform> AnimatedBoneTransforms;
	TArray<FBoneTransform> FinalBoneTransforms;
	int32 tot_len_of_bones;
	void GetResetedPoseInfo(FCSPose<FCompactPose>& MeshBases);
	void GetAnimatedPoseInfo(FCSPose<FCompactPose>& MeshBases);
	TArray<FBoneTransform> BoneTransforms;


	TArray<FCompactPoseBoneIndex> combined_indices;
	void Make_All_Bones(FCSPose<FCompactPose>& MeshBases);


	FRotator BoneRelativeConversion(FRotator feet_data, FCompactPoseBoneIndex ModifyBoneIndex, FRotator target_rotation, const FBoneContainer & BoneContainer, FCSPose<FCompactPose>& MeshBases);


	FRotator BoneInverseConversion(FCompactPoseBoneIndex ModifyBoneIndex, FRotator target_rotation, const FBoneContainer & BoneContainer, FCSPose<FCompactPose>& MeshBases);



	FVector GetCurrentLocation(FCSPose<FCompactPose>& MeshBases, const FCompactPoseBoneIndex& BoneIndex);


	FVector RotateAroundPoint(FVector input_point, FVector forward_vector, FVector origin_point, float angle);


//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FootAdvancedTweaks, meta = (DisplayName = "Foot Knee Pole Vectors", PinHiddenByDefault))
//		TArray<FVector> Foot_Knee_Array;



//	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpineAdvancedTweaks, meta = (DisplayName = "(optional) Dynamic Foot Height Offset Array", PinHiddenByDefault))
//		TArray<float> Foot_Height_Offset_Array;


	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DynamicFootOffset, meta = (DisplayName = "(optional) Foot 1 Height offset", PinHiddenByDefault))
		float Foot_01_Height_Offset = 0;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DynamicFootOffset, meta = (DisplayName = "(optional) Foot 2 Height offset", PinHiddenByDefault))
		 float Foot_02_Height_Offset = 0;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DynamicFootOffset, meta = (DisplayName = "(optional) Foot 3 Height offset", PinHiddenByDefault))
		 float Foot_03_Height_Offset = 0;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DynamicFootOffset, meta = (DisplayName = "(optional) Foot 4 Height offset", PinHiddenByDefault))
		 float Foot_04_Height_Offset = 0;




	// FAnimNode_Base interface
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
//	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)  override;
	//virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;

	virtual void UpdateInternal(const FAnimationUpdateContext& Context) override;

	//virtual void EvaluateComponentSpace_AnyThread(FComponentSpacePoseContext& Output) override;

	virtual void EvaluateComponentSpaceInternal(FComponentSpacePoseContext& Context) override;
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;


	virtual void Evaluate_AnyThread(FPoseContext& Output);


	// use this function to evaluate for skeletal control base
//	virtual void EvaluateComponentSpaceInternal(FComponentSpacePoseContext& Context);
	// Evaluate the new component-space transforms for the affected bones.
	//	virtual void EvaluateBoneTransforms(USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases, TArray<FBoneTransform>& OutBoneTransforms);

//	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms);


	void LineTraceControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms);


	// return true if it is valid to Evaluate
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones);
	// initialize any bone references you have
	virtual void InitializeBoneReferences(FBoneContainer& RequiredBones) override;



protected:


};

