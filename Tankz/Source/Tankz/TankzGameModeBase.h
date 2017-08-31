#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/TankzMapData.h"
#include "Data/TankTypeData.h"
#include <tuple>
#include <vector>

#include "TankzGameModeBase.generated.h"

class ATankzGameState;

/**
 * 
 */
UCLASS()
class TANKZ_API ATankzGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATankzGameModeBase();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<FTankTypeData> TankTypes;

	void SelectNextTank();
	void SelectPrevTank();
	void MarkThatTheSelectedTankHasActed();

private:
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
	 * This method will recalculate ActingTanks with those Tanks that have
	 * higher (or lower) initiative depending on Status.
	 * Returns:
	 *     - false: if all tanks has acted, so a change in Status was necesary.
	 *     - true: otherwise
	 *  but it will ALWAYS leave ActingTanks with at least one element.
	 */
	bool RecalculateActingTanks();

	void resetTanks();
	void incrementStatus();
	std::tuple<int32,bool> getFirstInitiative(TArray<ATankBase*> tanks) const;
	void SetActingTanksToAllTanksWithInitiative(int32 initiative, TArray<ATankBase*> tanks);

};
