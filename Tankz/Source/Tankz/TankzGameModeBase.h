#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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
};
