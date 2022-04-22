#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Minion.generated.h"

UCLASS()
class UGC_DEMO_API ACDoAction_Minion : public ACDoAction
{
	GENERATED_BODY()
public:
	virtual void DoAction() override;
	virtual void End_DoAction() override;

	virtual void DoEvade() override;
	virtual void End_DoEvade() override;

public:
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherChracter) override;
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherChracter) override;

private:
	UFUNCTION()
		void RestoreGlobalTimeDilation();


private:
	int32 Index;
	TArray<class ACharacter*> HittedCharacter;
};
