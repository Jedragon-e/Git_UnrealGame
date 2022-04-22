#include "CNotify_Hitted.h"
#include "Global.h"
#include "Components/CStateComponent.h"

FString UCNotify_Hitted::GetNotifyName_Implementation() const
{
	return "Hitted";
}

void UCNotify_Hitted::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(state);
	state->SetIdleMode();
}