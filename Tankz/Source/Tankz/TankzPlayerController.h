// No copyright notice because I don't like copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "TankzPlayerController.generated.h"

class ATankBase;
class UInplayUserWidgetBase;

/**
 * 
 */
UCLASS()
class TANKZ_API ATankzPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ATankzPlayerController();
	
protected:
	virtual void SetupInputComponent() override;
	virtual void PostInitializeComponents() override;
private:
	UInplayUserWidgetBase* dialogueHUD;
};
