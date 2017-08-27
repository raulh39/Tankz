#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/TankzMapData.h"
#include "Data/TankTypeData.h"
#include "TankzGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TANKZ_API ATankzGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATankzGameModeBase();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<FTankTypeData> TankTypes;
	
private:

	FTankzMapData LoadJson();
	void Spawn(FTankData tank, bool isAttacker);
};
