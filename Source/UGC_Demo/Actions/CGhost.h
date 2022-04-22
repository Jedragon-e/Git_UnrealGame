#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "CGhost.generated.h"

UCLASS()
class UGC_DEMO_API ACGhost : public AActor
{
	GENERATED_BODY()

public:
	ACGhost();
	virtual void Tick(float DeltaTime) override;

public:
	void Init(USkeletalMeshComponent* Pawn);

private:
	UFUNCTION() void OnTimeline(float Value);
	UFUNCTION() void OnTimelineFinish();

private:
	class USkeletalMesh* skeletalMesh;
	class UPoseableMeshComponent* PoseableMesh;
	class UMaterialInstanceConstant* GhostMaterial;
	TArray<class UMaterialInstanceDynamic*> Materials;

	// TimeLine
	class UCurveFloat* Curve;
	FTimeline Timeline;
};
