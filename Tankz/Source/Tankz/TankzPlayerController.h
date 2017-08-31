// No copyright notice because I don't like copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

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
	void ResetSelected();
protected:
	virtual void SetupInputComponent();
};
