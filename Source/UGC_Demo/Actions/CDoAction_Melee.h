#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Melee.generated.h"

UCLASS()
class UGC_DEMO_API ACDoAction_Melee : public ACDoAction
{
	GENERATED_BODY()
public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DiableCombo() { bEnable = false; }

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void DoEvade() override;
	virtual void End_DoEvade() override;

	virtual void DoCounter() override;
	virtual void End_DoCounter() override;

public:
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherChracter) override;
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherChracter) override;

private:
	UFUNCTION()
		void RestoreGlobalTimeDilation();

private:
	int32 Index;

	bool bEnable;
	bool bExist;
	bool bLast;

	TArray<class ACharacter*> HittedCharacter;
	
};
