#include "CNotify_Dead.h"
#include "Global.h"
#include "Components/CActionComponent_Minion.h"
#include "Character/ICharacter.h"

FString UCNotify_Dead::GetNotifyName_Implementation() const
{
	return "Dead";
}

void UCNotify_Dead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);

	character->End_Dead();
}