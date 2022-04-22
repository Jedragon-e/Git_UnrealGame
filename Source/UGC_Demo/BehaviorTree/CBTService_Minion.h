#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Minion.generated.h"

UCLASS()
class UGC_DEMO_API UCBTService_Minion : public UBTService
{
	GENERATED_BODY()

private:
	UCBTService_Minion();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
