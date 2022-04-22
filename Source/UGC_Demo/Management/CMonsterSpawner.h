#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CMonsterSpawner.generated.h"

USTRUCT(BlueprintType)
struct FMonsterSpawnData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		int32 Stage;
	UPROPERTY(EditAnywhere)
		int32 MinionCount;
	UPROPERTY(EditAnywhere)
		int32 LaneCount;
	UPROPERTY(EditAnywhere)
		int32 SuperCount;
};

UCLASS()
class UGC_DEMO_API ACMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ACMonsterSpawner();

protected:
	virtual void BeginPlay() override;

//private:
//	UPROPERTY(BlueprintReadOnly)
//		TSubclassOf<class UUserWidget> SelectItemWidgetClass;
//	class UUserWidget* SelectItemWidget;

public:
	void Spawn(int32 index);	
	void SelectItem();
	UFUNCTION()
		void StartStage();
	UFUNCTION(BlueprintCallable)
		void SetStage();	
	UFUNCTION()
		void ClearCount();

private:
	void BeginSpawn();

protected:
	UPROPERTY(EditAnywhere)
		TArray<AActor*> SpawnPositions;
	UPROPERTY(EditAnywhere)
		AActor* PlayerStartPos;

private:
	UPROPERTY()
		TSubclassOf<class ACEnemy_AI> MinionClass;	
	UPROPERTY()
		TSubclassOf<class ACEnemy_AI> LaneClass;	
	UPROPERTY()
		TSubclassOf<class ACEnemy_AI> SuperClass_AI;
	UPROPERTY()
		TSubclassOf<class AController> AiController;
	int32 monsterCount = 0;

private:
	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* DataTable;
private:
	int32 StageNum = 0;
	TArray<FMonsterSpawnData*> Datas;
};
