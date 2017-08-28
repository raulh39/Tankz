// No copyright notice because I don't like copyright

#pragma once

#include "TankState.generated.h"

USTRUCT(BlueprintType)
struct TANKZ_API FTankState
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY() FString name;
	UPROPERTY() int32 initiative;
	UPROPERTY() int32 attack;
	UPROPERTY() int32 defence;
	UPROPERTY() int32 damage_capacity;
	UPROPERTY() TArray<FString> habilities;
};
