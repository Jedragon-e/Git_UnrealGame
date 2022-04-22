#include "CBTService_Minion.h"
#include "Global.h"

//---------------------------------------------------------

#include "Character/CEnemy_AI.h"
#include "Character/CPlayer.h"
#include "Character/CAIController.h"
#include "Components/CStateComponent.h"
#include "Components/CBehaviorComponent.h"

UCBTService_Minion::UCBTService_Minion()
{
	NodeName = "CMinion";
}

void UCBTService_Minion::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);

	CheckTrue(state->IsDeadMode());

	if (state->IsHittedMode())
	{
		behavior->SetHittedMode();
		return;
	}

	ACPlayer* target = behavior->GetTargetPlayer();

	if (target == nullptr)
	{
		behavior->SetWaitMode();
		return;
	}

	float distance = aiPawn->GetDistanceTo(target);
	// �� ��°�� ȸ���� ���� �ʾƼ� ���� ������ ���ΰ�???
	//aiPawn->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(aiPawn->GetActorLocation(), target->GetActorLocation()));
	//aiPawn->MoveToRotator(UKismetMathLibrary::FindLookAtRotation(aiPawn->GetActorLocation(), target->GetActorLocation()));
	if (distance < controller->GetBehaviorRange())
	{
		behavior->SetActionMode();
		return;
	}

	if (distance < controller->GetSightRadius())
	{
		behavior->SetApproachMode();
		return;
	}
}
