#include "CNotifyState_Collision_Minion.h"
#include "Global.h"

#include "Components/CActionComponent_Minion.h"
#include "Actions_Minion/CActionData_Minion.h"
#include "Actions_Minion/CDoAction_Minion.h"
#include "Actions/CAttachment.h"

FString UCNotifyState_Collision_Minion::GetNotifyName_Implementation() const
{
	return "Collision_Minion";
}

void UCNotifyState_Collision_Minion::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent_Minion* action = CHelpers::GetComponent<UCActionComponent_Minion>(MeshComp->GetOwner());
	CheckNull(action);

	action->GetData()->GetAttachment()->OnCollision();
}

void UCNotifyState_Collision_Minion::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	UCActionComponent_Minion* action = CHelpers::GetComponent<UCActionComponent_Minion>(MeshComp->GetOwner());
	CheckNull(action);

	action->GetData()->GetAttachment()->OffCollision();
}
