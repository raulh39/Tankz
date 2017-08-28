// No copyright notice because I don't like copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Actors/TankBase.h"

#include "TankzGameState.generated.h"

/**
 * 
 */
UCLASS()
class TANKZ_API ATankzGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category="Armies")
	TArray<ATankBase*> Attackers;

	UPROPERTY(VisibleAnywhere, Category="Armies")
	TArray<ATankBase*> Defenders;
};
