#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "CWidget_Item.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName Info;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UTexture* Texture;
};

UCLASS()
class UGC_DEMO_API UCWidget_Item : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
		void SetItem();
	void SetItem_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void ResetItem();
	void ResetItem_Implementation();

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "DataTable")
		UDataTable* DataTable;

	UPROPERTY(BlueprintReadWrite)
		class UTexture* image;
};
