// No copyright notice because I don't like copyright

#pragma once

#include "TankData.generated.h"

USTRUCT(BlueprintType)
struct TANKZ_API FTankData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY() FString name;
	UPROPERTY() FString mesh;
	UPROPERTY() int32 initiative;
	UPROPERTY() int32 attack;
	UPROPERTY() int32 defence;
	UPROPERTY() int32 damage_capacity;
	UPROPERTY() TArray<FString> habilities;
	UPROPERTY() int32 position_x;
	UPROPERTY() int32 position_y;
	UPROPERTY() int32 rotation;
};
