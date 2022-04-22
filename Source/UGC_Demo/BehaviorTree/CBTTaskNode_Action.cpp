#include "CBTTaskNode_Action.h"
#include "Global.h"

#include "Components/CActionComponent_Minion.h"
#include "Components/CStateComponent.h"
#include "Character/CAIController.h"
#include "Character/CEnemy_AI.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = "Action";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());

	UCActionComponent_Minion* action = CHelpers::GetComponent<UCActionComponent_Minion>(aiPawn);
	//CheckNull(action);
	//CLog::Print(action->GetFName().ToString());
	TotalTime = 0.0f;
	action->DoAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	TotalTime += DeltaSeconds;

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);

	if (state->IsIdleMode() && TotalTime > Delay)
	{
		// 성공 반환하는 함수
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}