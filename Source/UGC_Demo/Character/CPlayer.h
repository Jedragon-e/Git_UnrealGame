#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Components/TimelineComponent.h"
#include "CPlayer.generated.h"



UCLASS()
class UGC_DEMO_API ACPlayer : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		class UCurveFloat* Curve;
	UPROPERTY(EditDefaultsOnly, Category = "TeamID")
		uint8 TeamID = 0;

private: //Scene Component
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

private: //Actor Component
	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;	
	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;
	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action;

private: // Widget
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCWidget_Player> WidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCWidget_Selecter> ItemWidgetClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACGhost> Ghost;

public:
	UPROPERTY(BlueprintReadOnly)
		class UCWidget_Player* PlayerWidget;
	UPROPERTY(BlueprintReadOnly)
		class UCWidget_Selecter* ItemWidget;

public:
	ACPlayer(); 
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	FORCEINLINE bool GetSpace() { return bSpace; }

	UFUNCTION()
		void OnItemWidget();	
	UFUNCTION(BlueprintCallable)
		void OffItemWidget();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FGenericTeamId GetGenericTeamId() const;


	// ���� �������� �÷��̾ �⺻���·� �ٲٱ� ����.
	UFUNCTION() void OnUnarmed();

private:
	// Ű �Է����� ���� �Լ�
	// Axis
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis); 
	void OnZoomCtrl(float InAxis);

	// Action
	void OnWalk();
	void OffWalk();
	UFUNCTION() void OnTimeLine_WalkSpeed(float Value);

	void OnDoAction();
	void OnDoAction_pp();

	void OnSpace();
	void OffSpace();

	UFUNCTION() void OnWarrior();
	UFUNCTION() void OnBerserker();
	UFUNCTION() void PlayGhost();

private:
	UPROPERTY(EditDefaultsOnly)
		class UAnimMontage* Block_AnimMontage;

private: // Ÿ�Զ��� ���� ����
	FTimeline Timeline;
	FOnTimelineFloat TimelineFoat; 

	bool bSpace;

	class AController* DamageInstigator;
};
