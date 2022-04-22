#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDamageText.generated.h"

UCLASS()
class UGC_DEMO_API ACDamageText : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDamageText();

	void SetTextValue(float value);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void TimeToDestory();

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UWidgetComponent* WidgetText;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		float Value;
};
