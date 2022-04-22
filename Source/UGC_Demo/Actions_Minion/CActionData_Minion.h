#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Actions/CAction.h"
#include "CActionData_Minion.generated.h"

UCLASS()
class UGC_DEMO_API UCActionData_Minion : public UDataAsset
{
	GENERATED_BODY()

public:
	void BeginPlay(class ACharacter* InOwnerCharacter, class UCAction** OutAction);

private:
	FString GetLabelName(class ACharacter* InOwnerCharacter, FString InName);

//public:
//	FORCEINLINE class ACDoAction* GetDoAction() { return DoAction; }
//	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACDoAction> DoActionClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoActionData> DoActionDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FEquipmentData SpawnData;
//private:
//	class ACAttachment* Attachment;
//	class ACDoAction* DoAction;
};
