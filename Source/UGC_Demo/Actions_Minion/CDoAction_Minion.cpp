#include "CDoAction_Minion.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

void ACDoAction_Minion::DoAction()
{
	Super::DoAction();

	CheckFalse(State->IsIdleMode());
	State->SetActionMode();
	//CLog::Print("In CDoaction_Minion :: DoAction");

	Index = UKismetMathLibrary::RandomIntegerInRange(0, Datas.Num() - 1);
	//CLog::Print(Index);
	//CLog::Print(Datas.Num() - 1);

	OwnerCharacter->PlayAnimMontage(Datas[Index].AnimMontage, Datas[Index].PlayRate, Datas[Index].StartSection);
	Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Minion::End_DoAction()
{
	Super::End_DoAction();

	OwnerCharacter->StopAnimMontage(Datas[Index].AnimMontage);

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Minion::DoEvade()
{
	Super::DoEvade();

	State->SetEvadeMode();

	OwnerCharacter->PlayAnimMontage(EvadeData.AnimMontage, EvadeData.PlayRate, EvadeData.StartSection);
	Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Minion::End_DoEvade()
{
	Super::End_DoEvade();

	OwnerCharacter->StopAnimMontage(EvadeData.AnimMontage);

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Minion::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherChracter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherChracter);

	CheckNull(InOtherChracter);

	//CLog::Print(InOtherChracter->GetActorLabel());

	// 한번 피격된 캐릭터는 충돌 처리에서 제외
	for (const ACharacter* other : HittedCharacter)
	{
		if (other == InOtherChracter)
			return;
	}
	HittedCharacter.Add(InOtherChracter);

	// 히트 스탑
	float hitStop = Datas[Index].HitStop;
	if (!FMath::IsNearlyZero(hitStop))
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreGlobalTimeDilation", hitStop * 2e-2f, false);
	}

	// 히트 파티클
	UParticleSystem* hitEffect = Datas[Index].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[Index].EffectTransform;
		transform.AddToTranslation(InOtherChracter->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	// 실제 대미지 
	FDamageEvent e;
	InOtherChracter->TakeDamage(Datas[Index].Power, e, InAttacker->GetController(), InAttackCauser);
}

void ACDoAction_Minion::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherChracter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackCauser, InOtherChracter);
	HittedCharacter.Empty();

}

void ACDoAction_Minion::RestoreGlobalTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
}
