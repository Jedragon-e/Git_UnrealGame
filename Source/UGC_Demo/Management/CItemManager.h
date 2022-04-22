#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CItemManager.generated.h"

UCLASS()
class UGC_DEMO_API ACItemManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ACItemManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
