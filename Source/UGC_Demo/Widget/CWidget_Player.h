#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWidget_Player.generated.h"

UCLASS()
class UGC_DEMO_API UCWidget_Player : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnPlayBloodScreen();

	UFUNCTION(BlueprintNativeEvent)
		void OnPlayFadeScreen();
	void OnPlayFadeScreen_Implementation();

protected:
	UPROPERTY(BlueprintReadOnly)
		bool bFadeInOut;
};
