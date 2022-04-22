#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWidget_DamagetText.generated.h"

UCLASS()
class UGC_DEMO_API UCWidget_DamagetText : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
		void OnPlayDamageText();
	void OnPlayDamageText_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
		void SetValueAFC(int32 value);

};
