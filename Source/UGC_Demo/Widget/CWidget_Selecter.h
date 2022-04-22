
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWidget_Selecter.generated.h"

UCLASS()
class UGC_DEMO_API UCWidget_Selecter : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
		void OnStartAnimation();
	void OnStartAnimation_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void OnResetItem();
	void OnResetItem_Implementation();
};
