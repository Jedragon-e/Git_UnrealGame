#include "CEnemy.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

//#include "Materials/MaterialInstanceConstant.h"
//#include "Materials/MaterialInstanceDynamic.h"

#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent_Minion.h"

#include "Objects/CDamageText.h"

ACEnemy::ACEnemy()
{
	//Create ActorComponent
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &Action, "Action");

	//Component Setting
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.0f), FRotator(0, -90, 0)); 
	
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/ParagonMinions/Characters/Minions/White_Camp_Minion/Meshes/Minion_melee.Minion_melee'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> anim;
	CHelpers::GetClass<UAnimInstance>(&anim, "AnimBlueprint'/Game/Enemy/ABP_Minion.ABP_Minion_C'");
	GetMesh()->SetAnimInstanceClass(anim);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();

	CHelpers::GetClass<ACDamageText>(&DamageTextClass, "Blueprint'/Game/Objects/BP_CDamageText.BP_CDamageText_C'");

	//PrimaryActorTick.bCanEverTick = true;
	//CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/Actions/CV_Ghost.CV_Ghost'");

}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	//UMaterialInstanceConstant* dissolve;
	//CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&dissolve, "MaterialInstanceConstant'/Game/Materials/Dissolve/MAT_Dissolve_Inst.MAT_Dissolve_Inst'");
	//DeadMaterial = UMaterialInstanceDynamic::Create(dissolve, this);

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChange);
	CHelpers::GetAssetDynamic<UParticleSystem>(&DeadEffect, "ParticleSystem'/Game/Particle/Fire_Exp_00.Fire_Exp_00'");

	//TimelineFoat.BindUFunction(this, "OnTimeline");
	//Timeline.AddInterpFloat(Curve, TimelineFoat);
	//Timeline.SetPlayRate(1.0f);	//배속

	//DeadDissolve();
	//Timeline.ReverseFromEnd();
}

//void ACEnemy::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	Timeline.TickTimeline(DeltaTime);
//}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	DamageInstigator = EventInstigator;

	//TODO::
	FTransform transform;
	ACDamageText* damageText = nullptr;
	damageText = GetWorld()->SpawnActorDeferred<ACDamageText>(DamageTextClass, GetActorTransform());
	damageText->SetTextValue(Damage);
	UGameplayStatics::FinishSpawningActor(damageText, GetActorTransform());




	Status->SubHealth(Damage);
	if (Status->GetHealth() <= 0)
	{
		State->SetDeadMode();
		return 0.0f;
	}
	State->SetHittedMode();

	return Status->GetHealth();
}

void ACEnemy::Hitted()
{
	Status->SetMove();
	Montages->PlayHitted();

	FVector start = GetActorLocation();
	FVector target = DamageInstigator->GetPawn()->GetActorLocation();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));

	FVector direction = target - start;
	direction.Normalize();
	LaunchCharacter(-direction * LaunchValue, true, false);

}

void ACEnemy::Dead()
{
	CheckFalse(State->IsDeadMode());

	//CLog::Print(GetActorLabel() + "이 죽었어요.");
	Action->Dead();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Timeline.PlayFromStart();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeadEffect, GetActorLocation(),GetActorRotation(),FVector(.5f,.5f,.5f));
	Montages->PlayDead();
}

void ACEnemy::End_Dead()
{
	Action->End_Dead();

	if (OnDeadDelegate.IsBound())
		OnDeadDelegate.Broadcast();

	Destroy();
}

void ACEnemy::OnStateTypeChange(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hitted:
		Hitted();
		break;
	case EStateType::Dead:
		Dead();
		break;
	}
}

//void ACEnemy::DeadDissolve()
//{
//	//GetComponents<USkeletalMesh>(ShapeComponents);
//
//	int32 matCount = GetMesh()->GetNumMaterials();
//	CLog::Print(matCount);
//
//	for (int32 i = 0; i < matCount; i++)
//	{
//		GetMesh()->SetMaterial(i, DeadMaterial);
//	}
//}
//
//void ACEnemy::OnTimeline(float Value)
//{
//	DeadMaterial->SetScalarParameterValue("Dissolve_Value", Value);
//}