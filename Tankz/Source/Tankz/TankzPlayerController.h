// No copyright notice because I don't like copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/DefaultPawn.h"
#include "TankzGameState.h"
#include "ActionElementWidgetBase.h"
#include <vector>

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
	void OnEsc();
	void OnMoveForward(float value);
	void OnMoveRight(float value);
	void OnMoveUp(float value);
	void OnPanX(float value);
	void OnPanY(float value);
	void ActivateFloatMode();
	void DeactivateFloatMode();
	
	UFUNCTION()
	void PhaseChange(TankzPhase newPhase);

	UFUNCTION()
	void SetActionList(TArray<FString> actionList);

	UFUNCTION()
	void HighlightAction(int actionNum, bool activated);

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
private:
	UInplayUserWidgetBase* dialogueHUD;
	TSubclassOf<UActionElementWidgetBase> actionElementWidgetBase;
	ATankzGameModeBase *gameMode;
	bool floatMode;
	ADefaultPawn* pawn;
	std::vector<UActionElementWidgetBase*> tankActions;
	UActionElementWidgetBase* CreateAction(FString actionName);
};
