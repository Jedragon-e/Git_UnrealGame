#include "CGhost.h"
#include "Global.h"
#include "Components/PoseableMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACGhost::ACGhost()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &PoseableMesh, "PoseableMesh");
	CHelpers::GetAsset<UMaterialInstanceConstant>(&GhostMaterial, "MaterialInstanceConstant'/Game/Materials/Ghost/MAT_Ghost_Inst.MAT_Ghost_Inst'");
	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/Actions/CV_Ghost.CV_Ghost'");

	CHelpers::GetAsset<USkeletalMesh>(&skeletalMesh, "SkeletalMesh'/Game/Character/Mesh/Mannequin/SK_Mannequin.SK_Mannequin'");
	PoseableMesh->SetSkeletalMesh(skeletalMesh);

	FOnTimelineFloat TimelineFoat;
	TimelineFoat.BindUFunction(this, "OnTimeline");
	FOnTimelineEvent FinishTimeLine;
	FinishTimeLine.BindUFunction(this, "OnTimelineFinish");

	Timeline.AddInterpFloat(Curve, TimelineFoat);
	Timeline.SetTimelineFinishedFunc(FinishTimeLine);
	Timeline.SetLooping(false);
	Timeline.SetPlayRate(1.0f);	//¹è¼Ó
}

void ACGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Timeline.TickTimeline(DeltaTime);
}

void ACGhost::Init(USkeletalMeshComponent* Pawn)
{
	PoseableMesh->CopyPoseFromSkeletalComponent(Pawn);

	for (int32 i = 0; i < PoseableMesh->GetMaterials().Num(); i++)
	{
		UMaterialInstanceDynamic* matDymaic = UMaterialInstanceDynamic::Create(GhostMaterial, this);
		Materials.Add(matDymaic);
		PoseableMesh->SetMaterial(i, matDymaic);
	}

	Timeline.PlayFromStart();
}

void ACGhost::OnTimeline(float Value)
{
	for (UMaterialInstanceDynamic* mat : Materials)
		mat->SetScalarParameterValue("Opacity", Value);
}

void ACGhost::OnTimelineFinish()
{
	Destroy();
}

