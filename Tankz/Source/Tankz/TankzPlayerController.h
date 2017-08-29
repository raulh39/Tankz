// No copyright notice because I don't like copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankzGameState.h"
#include <vector>
#include "TankzPlayerController.generated.h"

class ATankBase;

/**
 * 
 */
UCLASS()
class TANKZ_API ATankzPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void ResetStatus();
private:
	ATankzGameState* gameState;
	std::vector<ATankBase*> NextGroupOfTanksToAct;
	std::vector<ATankBase*>::size_type CurrentlySelectedTank;
};
