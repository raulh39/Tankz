#include <iostream>
#include <memory>

enum EStates
{
	SelectingTankToFire,
	SelectingTarget,
	EndState
};

class GameState;

class GameMode
{
public:
	GameMode();
	//Events:
	void OnCycle();
	void OnSelect();

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

	EStates currentState() const { return current_state; }

private:
	EStates current_state;
	std::unique_ptr<GameState> states[EndState];
	using mptr = EStates (GameState::*)(GameMode&);
	void Exec(mptr func);

	bool MoreTanksToFireExecuted=false;
};

class GameState
{
public:
	virtual ~GameState() {}
	virtual void OnEntry(GameMode&  game_mode) {}
	virtual void OnExit(GameMode&  game_mode) {}

	virtual EStates OnCycle(GameMode&  game_mode) { return game_mode.currentState(); }
	virtual EStates OnSelect(GameMode& game_mode) { return game_mode.currentState(); }
};

void GameMode::OnCycle() {
	Exec(&GameState::OnCycle);
}

void GameMode::OnSelect() {
	Exec(&GameState::OnSelect);
}

void GameMode::Exec(mptr func) {
	if(current_state==EndState) {
		std::cout << "Not executing because in end state\n";
		return;
	}
	states[current_state]->OnExit(*this);
	current_state = (*states[current_state].*func)(*this);
	if(current_state==EndState) {
		std::cout << "Machine stoped\n";
		return;
	}
	states[current_state]->OnEntry(*this);
}

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
