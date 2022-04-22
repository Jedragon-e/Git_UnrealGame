#include "CActionComponent_Minion.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"
#include "Actions_Minion/CActionData_Minion.h"
#include "Actions/CAction.h"
#include "Actions/CAttachment.h"
#include "Actions/CEquipment.h"
#include "Actions/CDoAction.h"

UCActionComponent_Minion::UCActionComponent_Minion()
{
}


void UCActionComponent_Minion::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* character = Cast<ACharacter>(GetOwner());
	DataAsset->BeginPlay(character, &Data);
	
	DoEvade();
}

void UCActionComponent_Minion::DoAction()
{
	ACDoAction* action = Data->GetDoAction();
	//CLog::Print("DoAction_Minion");
	action->DoAction();
	//Data->GetDoAction()->DoAction();
}

void UCActionComponent_Minion::DoEvade()
{
	ACDoAction* action = Data->GetDoAction();
	action->DoEvade();
}

void UCActionComponent_Minion::OffAllCollision()
{

	if (!!Data->GetAttachment() == false)
		return;

	Data->GetAttachment()->OffCollision();
}

void UCActionComponent_Minion::Dead()
{
	OffAllCollision();
}

void UCActionComponent_Minion::End_Dead()
{
	if (!!Data && !!Data->GetAttachment())
		Data->GetAttachment()->Destroy();

	if (!!Data && !!Data->GetEquipment())
		Data->GetEquipment()->Destroy();

	if (!!Data && !!Data->GetDoAction())
		Data->GetDoAction()->Destroy();
}
