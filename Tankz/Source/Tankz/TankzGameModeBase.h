#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/TankzMapData.h"
#include "Data/TankTypeData.h"
#include "Data/PositionInArrowInfo.h"
#include <tuple>
#include <vector>
#include "FSM/States.h"
#include "TankzGameState.h"
#include "Actors/CrosshairBase.h"

#include "TankzGameModeBase.generated.h"

class ATankzGameState;
class AArrow;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChange, TankzPhase, newPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdatedActionList, TArray<FString>, newActions);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHighlightAction, int, actionNum, bool, activated);

/**
 * 
 */
UCLASS()
class TANKZ_API ATankzGameModeBase : public AGameModeBase, public GameModeStateMachine
{
	GENERATED_BODY()
public:
	ATankzGameModeBase();
	~ATankzGameModeBase();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<FTankTypeData> TankTypes;

	FOnPhaseChange OnPhaseChange;
	FOnUpdatedActionList OnUpdatedActionList;
	FOnHighlightAction OnHighlightAction;
	
private:
	bool terminating;
	ATankzGameState * GameState;
	FTankzMapData LoadJson();
	int FindIndexFor(FString mesh);
	void Spawn(FTankData tank, bool isAttacker, ATankzGameState*state);

	/*
	 * This will hold:
	 *    - the next group of tanks that can move, if Status == Moving
	 *    - the next group of tanks that can fire, if Status == Attacking
	 *    - the next group of tanks that need to take command decissions, if Status == Command
	*/
	std::vector<ATankBase*> ActingTanks;
	std::vector<ATankBase*>::size_type SelectedTank;
	/*
	 * This will hold the group of crosshairs (placed atop tanks) that the selected tank can fire to
	*/
	using ObjectivesContainer_t = std::vector<std::pair<ATankBase*,ACrosshairBase*>>;
	ObjectivesContainer_t Objectives;
	ObjectivesContainer_t::size_type SelectedObjective;
	ACrosshairBase *SpawnCrossHair(ATankBase *tank);
	TSubclassOf<ACrosshairBase> CrosshairType;

	std::tuple<int32, bool> getFirstInitiative(TArray<ATankBase*> tanks) const;
	void SetActingTanksToAllTanksWithInitiative(int32 initiative, TArray<ATankBase*> tanks);

	AArrow* arrow;
	float positionInSplineBorderOfArrowBase;
	FRotator arrowRotation;
	PositionInArrowInfo positionInArrowInfo;
	bool currentPlayerIsAttacker;

	TArray<FString> currentTankActions;
	int selectedAction;
public:
	//Functions used by states in transtitions:
	virtual void AdjustArrowBase(const EvMove&)                            override;
	virtual void AdjustArrowHead(const EvPan&)                             override;
	virtual void AdjustTankPosition(const EvMove&)                         override;
	virtual void AssignDamageAndMarkTankHasActed(const EvSelect&)          override;
	virtual void CalculateTankCommandActions(const EvSelect&)              override;
	virtual void CalculateInitialTankPositionAlongArrow(const EvSelect&)   override;
	virtual void ExecuteSelectedActionAndMarkTankHasActed(const EvSelect&) override;
	virtual void IncSelected(const EvCycle&)                               override;
	virtual void IncSelectedAction(const EvCycle&)                         override;
	virtual void IncSelectedObjective(const EvCycle&)                      override;
	virtual void MarkTankHasActed(const EvEsc&)                            override;
	virtual void SwitchPhase(const EvEndPhase&)                            override;
	virtual void TurnTank90Degrees(const EvCycle&)                         override;

	//Functions used by states on entry and exit:
	virtual void CalculateNextGroup()                                      override;
	virtual void HighlightSelectedAction()                                 override;
	virtual void HighlightSelectedObjective()                              override;
	virtual void HighlightSelectedTank()                                   override;
	virtual void PlaceTankOnArrowSide()                                    override;
	virtual void PositionArrowBase()                                       override;
	virtual void SetTanksToNotActed()                                      override;
	virtual void SpawnArrow()                                              override;
	virtual void DeleteArrowAndPlaceMovToken()                             override;
	virtual void UnhighlightSelectedAction()                               override;
	virtual void UnhighlightSelectedObjective()                            override;
	virtual void UnhighlightSelectedTank()                                 override;
	virtual void SelectObjectivesGroup()                                   override;
	virtual void UnselectObjectivesGroup()                                 override;

	//Guards used by states:
	virtual bool ASideHasWon()      override;
	virtual bool MoreMovesLeft()    override;
	virtual bool MoreTanksInGroup() override;
	virtual bool MoreTanksToAct()   override;
};
