// No copyright notice because I don't like copyright

#pragma once

#include "Data/TankState.h"
#include "TankData.generated.h"

USTRUCT(BlueprintType)
struct TANKZ_API FTankData: public FTankState
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY() FString mesh;
	UPROPERTY() int32 position_x;
	UPROPERTY() int32 position_y;
	UPROPERTY() int32 rotation;
};
