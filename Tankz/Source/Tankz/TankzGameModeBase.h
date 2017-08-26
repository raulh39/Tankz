#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/TankzMapData.h"

#include "TankzGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TANKZ_API ATankzGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
private:
	FTankzMapData LoadJson();
	void Spawn(FTankData tank, bool isAttacker);
};
