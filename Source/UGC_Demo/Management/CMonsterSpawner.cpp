#include "CMonsterSpawner.h"
#include "GameFramework/Controller.h"
#include "Blueprint/UserWidget.h"

#include "Global.h"
#include "Character/CEnemy_AI.h" 
#include "Character/CPlayer.h" 
#include "Widget/CWidget_Player.h"
#include "Widget/CWidget_Selecter.h"


ACMonsterSpawner::ACMonsterSpawner()
{
	CHelpers::GetClass<ACEnemy_AI>(&MinionClass, "Blueprint'/Game/Enemy/AI_Minion/BP_CEnemy_AI.BP_CEnemy_AI_C'");
	CHelpers::GetClass<ACEnemy_AI>(&LaneClass, "Blueprint'/Game/Enemy/AI_Lane/BP_CLane_AI.BP_CLane_AI_C'");
	CHelpers::GetClass<ACEnemy_AI>(&SuperClass_AI, "Blueprint'/Game/Enemy/AI_Super/BP_CSupre_AI.BP_CSupre_AI_C'");
	CHelpers::GetClass<AController>(&AiController, "Blueprint'/Game/Enemy/AI_Minion/BP_CAIController.BP_CAIController_C'");

	//CHelpers::GetClass<UUserWidget>(&SelectItemWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Selecter.WB_Selecter_C'");
}

void ACMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	DataTable->GetAllRows<FMonsterSpawnData>("", Datas);

	BeginSpawn();
}

void ACMonsterSpawner::Spawn(int32 index)
{
	CLog::Log("ACMonsterSpawner::Spawn" + index);
	int32 rand = UKismetMathLibrary::RandomIntegerInRange(0, SpawnPositions.Max()-1);
	ACEnemy_AI* minion = nullptr;

	switch (index)
	{
	case 0:
		minion = GetWorld()->SpawnActorDeferred<ACEnemy_AI>(MinionClass, SpawnPositions[rand]->GetTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		//minion = GetWorld()->SpawnActor<ACEnemy_AI>(MinionClass, SpawnPositions[rand]->GetTransform());
		break;
	case 1:
		minion = GetWorld()->SpawnActorDeferred<ACEnemy_AI>(LaneClass, SpawnPositions[rand]->GetTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		//minion = GetWorld()->SpawnActor<ACEnemy_AI>(LaneClass, SpawnPositions[rand]->GetTransform());
		break;
	case 2:
		minion = GetWorld()->SpawnActorDeferred<ACEnemy_AI>(SuperClass_AI, SpawnPositions[rand]->GetTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		//minion = GetWorld()->SpawnActor<ACEnemy_AI>(SuperClass_AI, SpawnPositions[rand]->GetTransform());
		break;
	}

	minion->OnDeadDelegate.AddDynamic(this, &ACMonsterSpawner::ClearCount);

	minion->AIControllerClass = AiController;
	UGameplayStatics::FinishSpawningActor(minion, SpawnPositions[rand]->GetTransform());

	monsterCount++;
}

void ACMonsterSpawner::ClearCount()
{
	monsterCount--;
	CLog::Print(monsterCount);

	if (monsterCount <= 0)
		SelectItem();
}

void ACMonsterSpawner::SelectItem()
{
	ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->OnItemWidget();
}

void ACMonsterSpawner::SetStage()
{
	//CLog::Print("Spanwer::SetStage");
	ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->PlayerWidget->OnPlayFadeScreen();

	// 왜 이부분에서는 아무 동작을 안하는 걸까?

	UKismetSystemLibrary::K2_SetTimer(this, "StartStage", 2.0f, false);
}

void ACMonsterSpawner::StartStage()
{
	BeginSpawn();
	ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->PlayerWidget->OnPlayFadeScreen(); 
	player->ItemWidget->OnResetItem();
	player->SetActorLocation(PlayerStartPos->GetActorLocation());
	player->SetActorRotation(FRotator::ZeroRotator);
	player->OnUnarmed();
}

void ACMonsterSpawner::BeginSpawn()
{
	monsterCount = 0;

	for (int32 i = 0; i < Datas[StageNum]->MinionCount; i++) {
		Spawn(0);
	}
	for (int32 i = 0; i < Datas[StageNum]->LaneCount; i++) {
		Spawn(1);
	}
	for (int32 i = 0; i < Datas[StageNum]->SuperCount; i++) {
		Spawn(2);
	}

	StageNum++;
}