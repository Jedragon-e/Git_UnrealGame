#include "CNotify_EndAction_Minion.h"
#include "Global.h"

#include "Components/CActionComponent_Minion.h"
#include "Actions_Minion/CActionData_Minion.h"
#include "Actions_Minion/CDoAction_Minion.h"

FString UCNotify_EndAction_Minion::GetNotifyName_Implementation() const
{
	return "End_Action_Minion";
}
void UCNotify_EndAction_Minion::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent_Minion* action = CHelpers::GetComponent<UCActionComponent_Minion>(MeshComp->GetOwner());
	CheckNull(action);

	action->GetData()->GetDoAction()->End_DoAction();
}