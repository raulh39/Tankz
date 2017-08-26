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
	ATankzGameModeBase();
	virtual void BeginPlay() override;

	UPROPERTY()
	UStaticMesh * TankMesh;

	UPROPERTY()
	UStaticMesh * ShermanMesh;
	
private:
	FTankzMapData LoadJson();
	void Spawn(FTankData tank, bool isAttacker);
};
