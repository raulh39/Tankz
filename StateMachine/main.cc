#include <iostream>
#include "fsm.h"

class SelectingTankToFireState: public GameState
{
	virtual void OnEntry(GameMode& game_mode) {
		game_mode.HighlightSelectedTank();
	}
	virtual void OnExit(GameMode& game_mode) {
		game_mode.UnhighlightSelectedTank();
	}
	virtual EStates OnCycle(GameMode& game_mode) {
		game_mode.IncSelected();
		return SelectingTankToFire;
	}
	virtual EStates OnSelect(GameMode& game_mode) {
		game_mode.SelectObjectivesGroup();
		return SelectingTarget;
	}
};

class SelectingTargetState: public GameState
{
	virtual void OnEntry(GameMode& game_mode) {
		game_mode.HighlightSelectedObjective();
	}
	virtual void OnExit(GameMode& game_mode) {
		game_mode.UnhighlightSelectedObjective();
	}
	virtual EStates OnCycle(GameMode& game_mode) {
		game_mode.IncSelectedObjective();
		return SelectingTarget;
	}
	virtual EStates OnSelect(GameMode& game_mode) {
		game_mode.AssignDamage();
		if(game_mode.MoreTanksToFire())
			return SelectingTankToFire;
		return EndState;
	}
};

GameMode::GameMode()
{
	states[SelectingTankToFire] = std::make_unique<SelectingTankToFireState>();
	states[SelectingTarget] = std::make_unique<SelectingTargetState>();

	current_state = SelectingTankToFire;
	states[current_state]->OnEntry(*this);
}

int main()
{
	GameMode game;
	std::cout << "\nPressinng Cycle()\n";
	game.OnCycle();
	std::cout << "\nPressinng Select()\n";
	game.OnSelect();
	std::cout << "\nPressinng Cycle()\n";
	game.OnCycle();
	std::cout << "\nPressinng Cycle()\n";
	game.OnCycle();
	std::cout << "\nPressinng Select()\n";
	game.OnSelect();
	std::cout << "\nPressinng Cycle()\n";
	game.OnCycle();
	std::cout << "\nPressinng Cycle()\n";
	game.OnCycle();
	std::cout << "\nPressinng Select()\n";
	game.OnSelect();
	std::cout << "\nPressinng Cycle()\n";
	game.OnCycle();
	std::cout << "\nPressinng Select()\n";
	game.OnSelect();
	std::cout << "\nPressinng Cycle()\n";
	game.OnCycle();
	std::cout << "\nPressinng Select()\n";
	game.OnSelect();
}

void GameMode::HighlightSelectedTank()
{
	std::cout << "Executing HighlightSelectedTank\n";
}

void GameMode::UnhighlightSelectedTank()
{
	std::cout << "Executing UnhighlightSelectedTank\n";
}

void GameMode::IncSelected()
{
	std::cout << "Executing IncSelected\n";
}


void GameMode::SelectObjectivesGroup()
{
	std::cout << "Executing SelectObjectivesGroup\n";
}

void GameMode::HighlightSelectedObjective()
{
	std::cout << "Executing HighlightSelectedObjective\n";
}

void GameMode::UnhighlightSelectedObjective()
{
	std::cout << "Executing UnhighlightSelectedObjective\n";
}

void GameMode::IncSelectedObjective()
{
	std::cout << "Executing IncSelectedObjective\n";
}

void GameMode::AssignDamage()
{
	std::cout << "Executing AssignDamage\n";
}

bool GameMode::MoreTanksToFire()
{
	if(!MoreTanksToFireExecuted) {
		MoreTanksToFireExecuted=true;
		std::cout << "Returning true from MoreTanksToFire\n";
		return true;
	}
	std::cout << "Returning false from MoreTanksToFire\n";
	return false;
}
