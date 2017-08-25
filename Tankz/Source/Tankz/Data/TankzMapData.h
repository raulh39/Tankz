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

	FORCEINLINE FString ToString() const {
		FString attackerList{TEXT("[")};
		for(auto at: attacker) {
			attackerList += TEXT("{") + at.ToString() + TEXT("}, ");
		}
		if(attackerList.Len() > 2) attackerList = attackerList.LeftChop(2);
		attackerList += TEXT("]");

		FString defenderList{TEXT("[")};
		for(auto def: defender) {
			defenderList += TEXT("{") + def.ToString() + TEXT("}, ");
		}
		if(defenderList.Len() > 2) defenderList = defenderList.LeftChop(2);
		defenderList += TEXT("]");

		return FString::Printf(
			TEXT(
				"version: %s, "
				"attacker: %s, "
				"defender: %s"
			),
			*version,
			*attackerList,
			*defenderList
		);
	}
};
