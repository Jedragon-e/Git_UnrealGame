#include "CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/ShapeComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"


ACAttachment::ACAttachment()
{
	CHelpers::CreateComponent(this, &Scene, "Scene");

	PrimaryActorTick.bCanEverTick = true;
	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/Actions/CV_Weapon_Dissolve.CV_Weapon_Dissolve'");
}

void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	GetComponents<UStaticMeshComponent>(StaticMeshs);
	for (UStaticMeshComponent* staticMesh : StaticMeshs) {
		UMaterialInstanceConstant* mat = Cast<UMaterialInstanceConstant>(staticMesh->GetMaterial(0));
		Weapon_Materials.Add(UMaterialInstanceDynamic::Create(mat, this));

		staticMesh->SetMaterial(0, Weapon_Materials.Last());
	}

	GetComponents<UShapeComponent>(ShapeComponents);
	for (UShapeComponent* component : ShapeComponents)
	{
		component->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
		component->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);
	}

	OffCollision(); //최초 콜리전 제거
	Timeline.ReverseFromEnd();

	Super::BeginPlay();

	TimelineFoat.BindUFunction(this, "OnTimeline");
	Timeline.AddInterpFloat(Curve, TimelineFoat);
	Timeline.SetPlayRate(1.0f);	//배속
}
void ACAttachment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void ACAttachment::AttachTo(USceneComponent* InComponent, FName InSocketName)
{
	InComponent->AttachToComponent(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSocketName);
}

void ACAttachment::AttachToCollision(USceneComponent* InComponent, FName InSocketName)
{
	InComponent->AttachToComponent(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSocketName);
}

void ACAttachment::OnEquip_Implementation()
{
	Timeline.PlayFromStart();
}

void ACAttachment::OnUnequip_Implementation()
{
	Timeline.ReverseFromEnd();
}

void ACAttachment::OnCollision()
{
	for (UShapeComponent* component : ShapeComponents)
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACAttachment::OnCollision(FString select)
{
	for (UShapeComponent* component : ShapeComponents)
	{
		if (component->GetFName().ToString() == select) 
		{
			component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			break;
		}
	}
}

void ACAttachment::OffCollision()
{
	for (UShapeComponent* component : ShapeComponents)
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());

	if (OnAttachmentBeginOverlap.IsBound())
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));
}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OnAttachmentEndOverlap.IsBound())
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));
}

void ACAttachment::OnTimeline(float Value)
{
	for (UMaterialInstanceDynamic* mat : Weapon_Materials)
		mat->SetScalarParameterValue("Dissolve_Value", Value);
}

