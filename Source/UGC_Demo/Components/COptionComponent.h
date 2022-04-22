#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "COptionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UGC_DEMO_API UCOptionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float HorizontalLookRate = 90.0f;

	UPROPERTY(EditAnywhere)
		float VerticalLookRate = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Zoom")
		float ZoomSpeed = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "Zoom")
		FVector2D ZoomRange = FVector2D(100, 1000);
public:
	UCOptionComponent();
	FORCEINLINE float GetHorizontalLookRate() { return HorizontalLookRate; }
	FORCEINLINE float GetVerticalLookRate() { return VerticalLookRate; }

	FORCEINLINE float GetZoomSpeed() { return ZoomSpeed; }
	FORCEINLINE float GetMinZoomRange() { return ZoomRange.X; }
	FORCEINLINE float GetMaxZoomRange() { return ZoomRange.Y; }
protected:
	virtual void BeginPlay() override;
};
