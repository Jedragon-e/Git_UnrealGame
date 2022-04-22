#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Unarmed, Warrior, Berserker, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGC_DEMO_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		class UCActionData* DatasAssets[(int32)EActionType::Max];

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UCAction* GetCurrent() { return Datas[(int32)Type]; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsUnarmedMode() { return Type == EActionType::Unarmed; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsWarriorMode() { return Type == EActionType::Warrior; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsBerserkerMode() { return Type == EActionType::Berserker; }

public:	
	UCActionComponent();

	UFUNCTION(BlueprintCallable)void SetUnarmedMode();
	UFUNCTION(BlueprintCallable)void SetWarriorMode();
	UFUNCTION(BlueprintCallable)void SetBerserkerMode();

public:
	void DoAction();
	void DoEvade();
	void DoCounter();

public:
	UPROPERTY(BlueprintAssignable)
		FActionTypeChanged OnActionTypeChanged;

protected:
	virtual void BeginPlay() override;

private:
	void SetMode(EActionType InType);
	void ChangeType(EActionType InNewType);

private:
	UPROPERTY()
		class UCAction* Datas[(int32)EActionType::Max];
	EActionType Type;
};
