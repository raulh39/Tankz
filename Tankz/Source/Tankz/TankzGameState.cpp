// No copyright notice because I don't like copyright

#include "TankzGameState.h"

void ATankzGameState::resetTanks()
{
	for(auto tank: Attackers) {
		if(tank->isAlive)
			tank->hasActed = false;
	}
	for(auto tank: Defenders) {
		if(tank->isAlive)
			tank->hasActed = false;
	}
}

void ATankzGameState::incrementStatus()
{
	switch (Status)
	{
	case Moving:
		Status = Attacking;
		break;
	case Attacking:
		Status = Command;
		break;
	case Command:
		Status = Moving;
		break;
	}
}

std::tuple<int32,bool> ATankzGameState::getFirstInitiative(TArray<ATankBase*> tanks) const
{
	if(Status == Attacking) { //Greater initiative goes first
		int32 currentInitiative = -1;
		for(auto tank: tanks)
			if(!tank->hasActed && tank->CurrentState.initiative > currentInitiative)
				currentInitiative = tank->CurrentState.initiative;
		if(currentInitiative == -1)
			return std::make_tuple(0, false);
		return std::make_tuple(currentInitiative, true);
	}
	//In all other cases, greater initiative goes last
	int32 currentInitiative = 1000;
	for(auto tank: tanks)
		if(!tank->hasActed && (tank->CurrentState.initiative < currentInitiative))
			currentInitiative = tank->CurrentState.initiative;
	if(currentInitiative == 1000)
		return std::make_tuple(0, false);
	return std::make_tuple(currentInitiative, true);
}

std::vector<ATankBase*> ATankzGameState::getAllTanksWithInitiative(int32 initiative, TArray<ATankBase*> tanks) const
{
	std::vector<ATankBase*> to_return;
	for(auto tank: tanks) {
		if(!tank->hasActed && tank->CurrentState.initiative == initiative)
			to_return.push_back(tank);
	}
	return to_return;
}

std::vector<ATankBase*> ATankzGameState::GetNextGroupOfTanksToAct()
{
	int32 attackersFirstInitiative, defendersFirstInitiative;
	bool attackersCanAct, defendersCanAct;
	std::tie(attackersFirstInitiative, attackersCanAct) = getFirstInitiative(Attackers);
	std::tie(defendersFirstInitiative, defendersCanAct) = getFirstInitiative(Defenders);
	UE_LOG(LogTemp, Log, TEXT("Initiatives: %s/%d    %s/%d"), attackersCanAct?TEXT("true"):TEXT("false"), attackersCanAct, defendersCanAct?TEXT("true"):TEXT("false"), defendersFirstInitiative);
	if(!attackersCanAct && !defendersCanAct) {
		resetTanks();
		incrementStatus();
		return std::vector<ATankBase*>{};
	}
	if(!defendersCanAct)
		return getAllTanksWithInitiative(attackersFirstInitiative, Attackers);
	if(!attackersCanAct)
		return getAllTanksWithInitiative(defendersFirstInitiative, Defenders);
	if( (Status == Attacking && attackersFirstInitiative >= defendersFirstInitiative) ||
		(Status != Attacking && attackersFirstInitiative < defendersFirstInitiative) )
		return getAllTanksWithInitiative(attackersFirstInitiative, Attackers);
	return getAllTanksWithInitiative(defendersFirstInitiative, Defenders);
}
