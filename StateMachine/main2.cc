#include "fsm2.h"
#include <iostream>

using namespace idea2;

bool MoreTanksToFireExecuted = false;
//Functions used when transitioning states:
void HighlightSelectedTank();
void UnhighlightSelectedTank();
void IncSelected();

void SelectObjectivesGroup();
void HighlightSelectedObjective();
void UnhighlightSelectedObjective();
void IncSelectedObjective();
void AssignDamage();

//Guards used when transitioning states:
bool MoreTanksToFire();

struct SelectingTargetState: public GameMode::State
{
	SelectingTargetState() {
		HighlightSelectedObjective();
	}
	~SelectingTargetState()  {
		UnhighlightSelectedObjective();
	}
	virtual State* OnCycle()  {
		IncSelectedObjective();
		return new SelectingTargetState; //return SelectingTankToFire;
	}
	virtual State* OnSelect();
};

struct SelectingTankToFireState: public GameMode::State
{
	SelectingTankToFireState() {
		HighlightSelectedTank();
	}
	~SelectingTankToFireState()  {
		UnhighlightSelectedTank();
	}
	virtual State* OnCycle()  {
		IncSelected();
		return new SelectingTankToFireState; //return SelectingTankToFire;
	}
	virtual State* OnSelect() {
		SelectObjectivesGroup();
		return new SelectingTargetState;
	}

};

GameMode::State *SelectingTargetState::OnSelect() {
	AssignDamage();
	if(MoreTanksToFire()) return new SelectingTankToFireState;
	return new GameMode::State;
}

int main()
{
	GameMode game(new SelectingTankToFireState);
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


void HighlightSelectedTank()
{
	std::cout << "Executing HighlightSelectedTank\n";
}

void UnhighlightSelectedTank()
{
	std::cout << "Executing UnhighlightSelectedTank\n";
}

void IncSelected()
{
	std::cout << "Executing IncSelected\n";
}


void SelectObjectivesGroup()
{
	std::cout << "Executing SelectObjectivesGroup\n";
}

void HighlightSelectedObjective()
{
	std::cout << "Executing HighlightSelectedObjective\n";
}

void UnhighlightSelectedObjective()
{
	std::cout << "Executing UnhighlightSelectedObjective\n";
}

void IncSelectedObjective()
{
	std::cout << "Executing IncSelectedObjective\n";
}

void AssignDamage()
{
	std::cout << "Executing AssignDamage\n";
}

bool MoreTanksToFire()
{
	if(!MoreTanksToFireExecuted) {
		MoreTanksToFireExecuted=true;
		std::cout << "Returning true from MoreTanksToFire\n";
		return true;
	}
	std::cout << "Returning false from MoreTanksToFire\n";
	return false;
}
