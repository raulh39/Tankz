// No copyright notice because I don't like copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Actors/TankBase.h"
#include <vector>
#include <tuple>

#include "TankzGameState.generated.h"


/**
 * 
 */
UENUM()
enum TankzPhase
{
	TankzPhase_Moving      UMETA(DisplayName = "Moving"), 
	TankzPhase_Attacking   UMETA(DisplayName = "Attacking"),
	TankzPhase_Command     UMETA(DisplayName = "Command"), 
};

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

	UPROPERTY(VisibleAnywhere, Category="Armies")
	TEnumAsByte<TankzPhase> CurrentPhase;
};
