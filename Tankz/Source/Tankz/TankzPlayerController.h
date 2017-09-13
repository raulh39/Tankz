// No copyright notice because I don't like copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "TankzPlayerController.generated.h"

class ATankBase;
class UInplayUserWidgetBase;
class ATankzGameModeBase;

/**
 * 
 */
UCLASS()
class TANKZ_API ATankzPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ATankzPlayerController();
	
	void OnCycleUp();
	void OnCycleDown();
	void OnSelect();
	void OnMoveForward(float value);
	void OnMoveRight(float value);
	void OnPanX(float value);
	void OnPanY(float value);

protected:
	virtual void SetupInputComponent() override;
private:
	UInplayUserWidgetBase* dialogueHUD;
	ATankzGameModeBase *gameMode;
};
