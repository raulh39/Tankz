#pragma once

#include "Actors/TankBase.h"

#include "TankTypeData.generated.h"

USTRUCT()
struct TANKZ_API FTankTypeData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATankBase> Blueprint;
};
