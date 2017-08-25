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

	FORCEINLINE FString ToString() const {
		FString habList{TEXT("[")};
		for(auto hab: habilities) {
			habList += hab + TEXT(", ");
		}
		if(habList.Len() > 2) habList = habList.LeftChop(2);
		habList += TEXT("]");

		return FString::Printf(
			TEXT(
				"name: %s, "
				"mesh: %s, "
				"initiative: %d, "
				"attack: %d, "
				"defence: %d, "
				"damage_capacity: %d, "
				"habilities: %s, "
				"position_x: %d, "
				"position_y: %d, "
				"rotation: %d"
			),
			*name,
			*mesh,
			initiative,
			attack,
			defence,
			damage_capacity,
			*habList,
			position_x,
			position_y,
			rotation
		);
	}
};
