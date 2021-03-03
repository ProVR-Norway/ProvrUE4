/* Copyright (C) Eternal Monke Games - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Mansoor Pathiyanthra <codehawk64@gmail.com , mansoor@eternalmonke.com>, 2021
*/

#include "DragonAimSolverEditMode.h"
#include "AnimGraphNode_DragonAimSolver.h"
#include "IPersonaPreviewScene.h"
#include "Animation/DebugSkelMeshComponent.h"

#include "SceneManagement.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
//#include "Materials/MaterialExpressionConstant3Vector.h"



const FEditorModeID DragonAimSolverEditModes::DragonAimSolver("AnimGraph.DragonControl.DragonAimSolver");


void FDragonAimSolverEditMode::EnterMode(class UAnimGraphNode_Base* InEditorNode, struct FAnimNode_Base* InRuntimeNode)
{
	RuntimeNode = static_cast<FAnimNode_DragonAimSolver*>(InRuntimeNode);
	GraphNode = CastChecked<UAnimGraphNode_DragonAimSolver>(InEditorNode);

	FDragonControlBaseEditMode::EnterMode(InEditorNode, InRuntimeNode);
}

void FDragonAimSolverEditMode::ExitMode()
{
	RuntimeNode = nullptr;
	GraphNode = nullptr;

	FDragonControlBaseEditMode::ExitMode();
}

FVector FDragonAimSolverEditMode::GetWidgetLocation() const
{
	USkeletalMeshComponent* SkelComp = GetAnimPreviewScene().GetPreviewMeshComponent();



	if (GraphNode)
	{

		if (Elbow_Pole_Mode && elbow_arm_index>=0)
		{
			if (RuntimeNode->Elbow_Bone_Transform_Array.Num() > elbow_arm_index)
			{
				FVector pole_offset = RuntimeNode->Aiming_Hand_Limbs[elbow_arm_index].Elbow_Pole_Offset;

				return (RuntimeNode->Elbow_Bone_Transform_Array[elbow_arm_index].GetLocation() + pole_offset);
			}
		}
		else
		{

			if (GraphNode->ik_node.is_focus_debugtarget)
				return GraphNode->ik_node.Debug_LookAtLocation.GetLocation();
			else
			{
				if (RuntimeNode->knee_Animated_transform_array.Num() > spine_index)
				{
					if (RuntimeNode->dragon_input_data.FeetBones.Num() > spine_index)
					{
						return (RuntimeNode->knee_Animated_transform_array[spine_index].GetLocation() + RuntimeNode->dragon_input_data.FeetBones[spine_index].Knee_Direction_Offset);
					}
				}
			}

		}
	}

	return FVector(0, 0, 0);

}

FWidget::EWidgetMode FDragonAimSolverEditMode::GetWidgetMode() const
{
	return FWidget::WM_Translate;
}







struct HDragonAimElbowPoleHandleHitProxy : public HHitProxy
{
	DECLARE_HIT_PROXY()

	
	int32 elbow_index;

	HDragonAimElbowPoleHandleHitProxy(int32 elbow_index_in) : HHitProxy(HPP_World), elbow_index(elbow_index_in)
	{
	}

	// HHitProxy interface
	virtual EMouseCursor::Type GetMouseCursor() override { return EMouseCursor::CardinalCross; }
	// End of HHitProxy interface
};

IMPLEMENT_HIT_PROXY(HDragonAimElbowPoleHandleHitProxy, HHitProxy)





struct HDragonAimFootSolverHandleHitProxy : public HHitProxy
{
	DECLARE_HIT_PROXY()

	int32 spine_index;

	int32 foot_index;

	HDragonAimFootSolverHandleHitProxy(int32 spine_index_in, int32 foot_index_in) : HHitProxy(HPP_World), spine_index(spine_index_in), foot_index(foot_index_in)
	{
	}

	// HHitProxy interface
	virtual EMouseCursor::Type GetMouseCursor() override { return EMouseCursor::CardinalCross; }
	// End of HHitProxy interface
};

IMPLEMENT_HIT_PROXY(HDragonAimFootSolverHandleHitProxy, HHitProxy)


struct HDragonAimSolverHandleHitProxy : public HHitProxy
{
	DECLARE_HIT_PROXY()

	FTransform Debug_Transform;


	HDragonAimSolverHandleHitProxy(FTransform Debug_Transform_in): HHitProxy(HPP_World), Debug_Transform(Debug_Transform_in)
	{
	}

	// HHitProxy interface
	virtual EMouseCursor::Type GetMouseCursor() override { return EMouseCursor::CardinalCross; }
	// End of HHitProxy interface
};

IMPLEMENT_HIT_PROXY(HDragonAimSolverHandleHitProxy, HHitProxy)






void FDragonAimSolverEditMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	UDebugSkelMeshComponent* SkelComp = GetAnimPreviewScene().GetPreviewMeshComponent();


	const FMaterialRenderProxy* TargetMaterialProxy = GEngine->ArrowMaterialYellow->GetRenderProxy();
	PDI->SetHitProxy(new HDragonAimSolverHandleHitProxy(RuntimeNode->Debug_LookAtLocation));


	FTransform StartTransform = RuntimeNode->Debug_LookAtLocation;
	FVector Head_Transform = RuntimeNode->Head_Orig_Transform.GetLocation();
	{		
		const float Scale = View->WorldToScreen(StartTransform.GetLocation()).W * (4.0f / View->UnscaledViewRect.Width() / View->ViewMatrices.GetProjectionMatrix().M[0][0]);
		DrawSphere(PDI, StartTransform.GetLocation(), FRotator::ZeroRotator, FVector(8.0f) * Scale, 64, 64, TargetMaterialProxy, SDPG_Foreground);
		DrawDashedLine(PDI, Head_Transform, StartTransform.GetLocation(), FLinearColor::Black, 2, 5);
	}




	for (int32 i = 0; i < RuntimeNode->dragon_input_data.FeetBones.Num(); i++)
	{
		//NewObject<UMaterial>();
		//GEngine->ArrowMaterial;

		UMaterialInstanceDynamic *Knee_Mat = UMaterialInstanceDynamic::Create( GEngine->ArrowMaterial,GEngine->ArrowMaterial);
		Knee_Mat->SetVectorParameterValue("GizmoColor",FLinearColor(FColor::Cyan));
		

		const FMaterialRenderProxy* KneeMaterialProxy = Knee_Mat->GetRenderProxy();
		
		PDI->SetHitProxy(new HDragonAimFootSolverHandleHitProxy(i, 0));

		FTransform StartTransformKnee = FTransform::Identity;
		if (RuntimeNode->knee_Animated_transform_array.Num() > i)
		{
			StartTransformKnee = RuntimeNode->knee_Animated_transform_array[i];
			StartTransformKnee.AddToTranslation(RuntimeNode->dragon_input_data.FeetBones[i].Knee_Direction_Offset);

			const float Scale = View->WorldToScreen(StartTransformKnee.GetLocation()).W * (4.0f / View->UnscaledViewRect.Width() / View->ViewMatrices.GetProjectionMatrix().M[0][0]);
			DrawSphere(PDI, StartTransformKnee.GetLocation(), FRotator::ZeroRotator, FVector(8.0f) * Scale, 64, 64, KneeMaterialProxy, SDPG_Foreground);
			DrawDashedLine(PDI, RuntimeNode->knee_Animated_transform_array[i].GetLocation(), StartTransformKnee.GetLocation(), FLinearColor::Black, 2, 5);
		}
	}





	for (int32 i = 0; i < RuntimeNode->Aiming_Hand_Limbs.Num(); i++)
	{
		//NewObject<UMaterial>();
		//GEngine->ArrowMaterial;

		UMaterialInstanceDynamic* Knee_Mat = UMaterialInstanceDynamic::Create(GEngine->ArrowMaterial, GEngine->ArrowMaterial);
		Knee_Mat->SetVectorParameterValue("GizmoColor", FLinearColor(FColor::Orange));


		const FMaterialRenderProxy* ElbowMaterialProxy = Knee_Mat->GetRenderProxy();

		PDI->SetHitProxy(new HDragonAimElbowPoleHandleHitProxy(i));

		FTransform StartTransformKnee = FTransform::Identity;
		if (RuntimeNode->Elbow_Bone_Transform_Array.Num() > i)
		{
			StartTransformKnee = RuntimeNode->Elbow_Bone_Transform_Array[i];
			StartTransformKnee.AddToTranslation(RuntimeNode->Aiming_Hand_Limbs[i].Elbow_Pole_Offset);

			const float Scale = View->WorldToScreen(StartTransformKnee.GetLocation()).W * (4.0f / View->UnscaledViewRect.Width() / View->ViewMatrices.GetProjectionMatrix().M[0][0]);
			DrawSphere(PDI, StartTransformKnee.GetLocation(), FRotator::ZeroRotator, FVector(8.0f) * Scale, 64, 64, ElbowMaterialProxy, SDPG_Foreground);
			DrawDashedLine(PDI, RuntimeNode->Elbow_Bone_Transform_Array[i].GetLocation(), StartTransformKnee.GetLocation(), FLinearColor::Black, 2, 5);
		}
	}

	
	
	RuntimeNode->ConditionalDebugDraw(PDI, SkelComp);



	PDI->SetHitProxy(nullptr);
}




bool FDragonAimSolverEditMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click)
{
	bool bResult = FDragonControlBaseEditMode::HandleClick(InViewportClient, HitProxy, Click);

//	if ((GraphNode->ik_node.is_focus_debugtarget))
	{
		if (HitProxy != nullptr && HitProxy->IsA(HDragonAimSolverHandleHitProxy::StaticGetType()))
		{
			HDragonAimSolverHandleHitProxy* HandleHitProxy = static_cast<HDragonAimSolverHandleHitProxy*>(HitProxy);

			GraphNode->ik_node.is_focus_debugtarget = true;

			bResult = true;

			Elbow_Pole_Mode = false;
		}
	}
//	else
	{

		if (HitProxy != nullptr && HitProxy->IsA(HDragonAimFootSolverHandleHitProxy::StaticGetType()))
		{
			HDragonAimFootSolverHandleHitProxy* HandleHitProxy = static_cast<HDragonAimFootSolverHandleHitProxy*>(HitProxy);

			GraphNode->ik_node.is_focus_debugtarget = false;

			spine_index = HandleHitProxy->spine_index;
			foot_index = HandleHitProxy->foot_index;

			bResult = true;

			Elbow_Pole_Mode = false;

		}

	}



	if (HitProxy != nullptr && HitProxy->IsA(HDragonAimElbowPoleHandleHitProxy::StaticGetType()))
	{
		HDragonAimElbowPoleHandleHitProxy* HandleHitProxy = static_cast<HDragonAimElbowPoleHandleHitProxy*>(HitProxy);

		GraphNode->ik_node.is_focus_debugtarget = true;

		elbow_arm_index = HandleHitProxy->elbow_index;

		bResult = true;

		Elbow_Pole_Mode = true;
	}

	

	return bResult;
}



FName FDragonAimSolverEditMode::GetSelectedBone() const
{

	if (GraphNode->ik_node.is_focus_debugtarget)
	{
		if ((GraphNode->ik_node.dragon_input_data.FeetBones.Num() > spine_index))
			return GraphNode->ik_node.dragon_input_data.FeetBones[spine_index].Feet_Bone_Name;
	}

		return "None";

	//return GraphNode->;
}

// @todo: will support this since now we have LookAtLocation
void FDragonAimSolverEditMode::DoTranslation(FVector& InTranslation)
{


	if (Elbow_Pole_Mode && elbow_arm_index >= 0)
	{

		RuntimeNode->Aiming_Hand_Limbs[elbow_arm_index].Elbow_Pole_Offset += InTranslation;
		GraphNode->ik_node.Aiming_Hand_Limbs[elbow_arm_index].Elbow_Pole_Offset += InTranslation;

	}
	else
	{
		if (GraphNode->ik_node.is_focus_debugtarget)
		{
			GraphNode->ik_node.Debug_LookAtLocation.AddToTranslation(InTranslation);
			RuntimeNode->Debug_LookAtLocation.AddToTranslation(InTranslation);
			Target_Transform_Value = GraphNode->ik_node.Debug_LookAtLocation.GetLocation();
		}
		else
			if ((RuntimeNode->dragon_input_data.FeetBones.Num() > spine_index) && (GraphNode->ik_node.dragon_input_data.FeetBones.Num() > spine_index))
			{
				RuntimeNode->dragon_input_data.FeetBones[spine_index].Knee_Direction_Offset += InTranslation;
				GraphNode->ik_node.dragon_input_data.FeetBones[spine_index].Knee_Direction_Offset += InTranslation;
			}
	}

//	Target_Transform_Value = GraphNode->ik_node.Debug_LookAtLocation.GetLocation() + InTranslation;


}