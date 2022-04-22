#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOtherChracter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOtherChracter);

UCLASS()
class UGC_DEMO_API ACAttachment : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;
	
protected:
	UFUNCTION(BlueprintCallable)
		void AttachTo(class USceneComponent* InComponent, FName InSocketName);
	UFUNCTION(BlueprintCallable)
		void AttachToCollision(class USceneComponent* InComponent, FName InSocketName);


public:
	UFUNCTION(BlueprintNativeEvent)
		void OnEquip();
	void OnEquip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void OnUnequip();
	void OnUnequip_Implementation();

public:
	ACAttachment();

	void OnCollision();
	void OnCollision(FString select);
	void OffCollision();

protected:
	virtual void BeginPlay() override;
	UFUNCTION() void OnTimeline(float Value);

public:
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(BlueprintAssignable)
		FAttachmentBeginOverlap OnAttachmentBeginOverlap;
	UPROPERTY(BlueprintAssignable)
		FAttachmentEndOverlap OnAttachmentEndOverlap;


protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;
	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;
	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

private:
	TArray<class UShapeComponent*> ShapeComponents;
	TArray<class UStaticMeshComponent*> StaticMeshs;
	TArray<class UMaterialInstanceDynamic*> Weapon_Materials;

	class UCurveFloat* Curve;
	FTimeline Timeline;
	FOnTimelineFloat TimelineFoat;
	
};
