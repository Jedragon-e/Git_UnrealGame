#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/ICharacter.h"
#include "Components/CStateComponent.h"
#include "Components/TimelineComponent.h"
#include "CEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeadDelegate);

UCLASS()
class UGC_DEMO_API ACEnemy : public ACharacter, public IICharacter
{
	GENERATED_BODY()

protected://State, Status
	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;
	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent_Minion* Action;
	UPROPERTY(EditDefaultsOnly)
		float LaunchValue = 50;

public:
	ACEnemy();
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UFUNCTION()
		void OnStateTypeChange(EStateType InPrevType, EStateType InNewType);

protected:
	virtual void BeginPlay() override;
	//UFUNCTION() void OnTimeline(float Value);

//public:
//	virtual void Tick(float DeltaTime) override;

private:
	void Hitted();
	void Dead();
	virtual void End_Dead() override;

public:
	UPROPERTY(BlueprintAssignable)
		FDeadDelegate OnDeadDelegate;

	//void DeadDissolve();

private:
	UPROPERTY(VisibleDefaultsOnly)
		TSubclassOf<class ACDamageText> DamageTextClass;

private:
	class AController* DamageInstigator;
	class UParticleSystem* DeadEffect;

	//class UMaterialInstanceDynamic* DeadMaterial;

	//class UCurveFloat* Curve;
	//FTimeline Timeline;
	//FOnTimelineFloat TimelineFoat;
};
