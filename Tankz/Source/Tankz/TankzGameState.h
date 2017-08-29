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
enum Status
{
	Moving      UMETA(DisplayName = "Moving"), 
	Attacking   UMETA(DisplayName = "Attacking"),
	Command     UMETA(DisplayName = "Command"), 
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
	TEnumAsByte<Status> Status;

	/*
	 * This method will return:
	 *    - the next group of tanks that can move, if Status == Moving
	 *    - the next group of tanks that can fire, if Status == Attacking
	 *    - the next group of tanks that need to take command decissions, if Status == Command
	 * If the Status has changed because no more tanks can act, then it will return an empty collection.
	*/
	std::vector<ATankBase*> GetNextGroupOfTanksToAct();
private:
	void resetTanks();
	void incrementStatus();
	std::tuple<int32,bool> getFirstInitiative(TArray<ATankBase*> tanks) const;
	std::vector<ATankBase*> getAllTanksWithInitiative(int32 initiative, TArray<ATankBase*> tanks) const;
};
