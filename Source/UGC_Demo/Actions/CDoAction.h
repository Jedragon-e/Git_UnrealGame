#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CACtionData.h"
#include "CDoAction.generated.h"

UCLASS()
class UGC_DEMO_API ACDoAction : public AActor
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetData(TArray<FDoActionData> InDatas) { Datas = InDatas; }
	FORCEINLINE void SetEvadeData(FEquipmentData InData) { EvadeData = InData; }
	FORCEINLINE void SetCounterData(FEquipmentData InData) { CounterData = InData; }
	FORCEINLINE void SetEquipped(const bool* InEquipped) { bEquipped = InEquipped; }
	FORCEINLINE void SetbLookAt(bool InLookAt) { bLookAt = InLookAt; }


public:	
	ACDoAction();
	
public:
	virtual void DoAction() {}
	virtual void Begin_DoAction() {}
	virtual void End_DoAction() {}

	virtual void DoEvade() {}
	virtual void End_DoEvade() {}

	virtual void DoCounter() {}
	virtual void End_DoCounter() {}

public:
	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherChracter) {};
	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherChracter) {};

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override; 

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;
	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;
	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;	
	UPROPERTY(BlueprintReadOnly)
		class UCActionComponent* Action;

protected:
	FEquipmentData EvadeData;
	FEquipmentData CounterData;
	TArray<FDoActionData> Datas;
	const bool* bEquipped;

	bool bLookAt;
};
