#include "CDoAction_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Character/CPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"

void ACDoAction_Melee::DoAction()
{
	Super::DoAction();
	CheckFalse(Datas.Num() > 0);

	if (bEnable)
	{
		bEnable = false;
		bExist = true;
		return;
	}

	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse(bExist);
	bExist = false;

	OwnerCharacter->StopAnimMontage();

	Index++;
	Index = FMath::Clamp<int32>(Index, 0, Datas.Num() - 1);

	OwnerCharacter->PlayAnimMontage(Datas[Index].AnimMontage, Datas[Index].PlayRate, Datas[Index].StartSection);
	Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	OwnerCharacter->StopAnimMontage(Datas[Index].AnimMontage);
	Index = 0;

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Melee::DoEvade()
{
	Super::DoEvade();

	CheckFalse(State->IsIdleMode());
	State->SetEvadeMode();
	Index = 0;

	OwnerCharacter->bUseControllerRotationYaw = EvadeData.bPawnControl;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = !EvadeData.bPawnControl;

	//������ �̵� �Է����� ȸ��
	OwnerCharacter->SetActorRotation(UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetLastMovementInputVector()));

	OwnerCharacter->PlayAnimMontage(EvadeData.AnimMontage, EvadeData.PlayRate, EvadeData.StartSection);
	EvadeData.bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::End_DoEvade()
{
	Super::End_DoEvade();

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(player);
	//CLog::Print(player->GetSpace());
	if (player->GetSpace())
	{
		OwnerCharacter->PlayAnimMontage(EvadeData.AnimMontage, EvadeData.PlayRate, EvadeData.StartSection);
		EvadeData.bCanMove ? Status->SetMove() : Status->SetStop();
		return;
	}

	OwnerCharacter->bUseControllerRotationYaw = bLookAt;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = !bLookAt;

	OwnerCharacter->StopAnimMontage(EvadeData.AnimMontage);

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Melee::DoCounter()
{
	Super::DoCounter();
	CheckFalse(Action->IsWarriorMode());
	CheckFalse(State->IsEvadeMode());
	State->SetActionMode();
	Index = 0;

	OwnerCharacter->bUseControllerRotationYaw = CounterData.bPawnControl;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = !CounterData.bPawnControl;

	OwnerCharacter->SetActorRotation(UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetLastMovementInputVector()));

	OwnerCharacter->StopAnimMontage(EvadeData.AnimMontage);
	OwnerCharacter->PlayAnimMontage(CounterData.AnimMontage, CounterData.PlayRate, CounterData.StartSection);
	CounterData.bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::End_DoCounter()
{
	Super::End_DoCounter();

	OwnerCharacter->StopAnimMontage(CounterData.AnimMontage);

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Melee::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherChracter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherChracter);

	CheckNull(InOtherChracter);

	// �ѹ� �ǰݵ� ĳ���ʹ� �浹 ó������ ����
	for (const ACharacter* other : HittedCharacter)
	{
		if (other == InOtherChracter)
			return;
	}
	HittedCharacter.Add(InOtherChracter);

	// ��Ʈ ��ž
	float hitStop = Datas[Index].HitStop;
	if (!FMath::IsNearlyZero(hitStop))
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreGlobalTimeDilation", hitStop * 2e-2f, false);
	}

	// ��Ʈ ��ƼŬ
	UParticleSystem* hitEffect = Datas[Index].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[Index].EffectTransform;
		transform.AddToTranslation(InOtherChracter->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	// ī�޶� ����ũ
	TSubclassOf<UCameraShake> shake = Datas[Index].ShakeClass;
	if (!!shake)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!!controller)
			controller->PlayerCameraManager->PlayCameraShake(shake);
	}

	// ���� ����� 
	FDamageEvent e;
	InOtherChracter->TakeDamage(Datas[Index].Power, e, InAttacker->GetController(), InAttackCauser);
}

void ACDoAction_Melee::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherChracter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackCauser, InOtherChracter);
	HittedCharacter.Empty();

}


void ACDoAction_Melee::RestoreGlobalTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
}
