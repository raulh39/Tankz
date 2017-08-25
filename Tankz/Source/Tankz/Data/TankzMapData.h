// No copyright notice because I don't like copyright

#pragma once
#include "TankData.h"
#include "TankzMapData.generated.h"

USTRUCT(BlueprintType)
struct TANKZ_API FTankzMapData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY() FString version;
	UPROPERTY() TArray<FTankData> attacker;
	UPROPERTY() TArray<FTankData> defender;
};
