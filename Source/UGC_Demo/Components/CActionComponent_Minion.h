#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent_Minion.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGC_DEMO_API UCActionComponent_Minion : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		class UCActionData_Minion* DataAsset;
public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UCAction* GetData() { return Data; }

public:	
	UCActionComponent_Minion();

protected:
	virtual void BeginPlay() override;

public:
	void DoAction();

	void DoEvade();

	void Dead();
	void End_Dead();

	void OffAllCollision();

private:
	UPROPERTY()
		class UCAction* Data;
};
