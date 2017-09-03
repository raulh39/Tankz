#include <iostream>


enum EStates
{
	SelectingTankToFire,
	SelectingTarget,
	NumberOfEStates
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

private:
	friend class GameState;
	EStates current_state;
	GameState*states[NumberOfEStates];
	void ChangeState(EStates new_state);
};

class GameState
{
public:
	virtual ~GameState() {}
	virtual void OnEntry(GameMode&  game_mode) {}
	virtual void OnExit(GameMode&  game_mode) {}

	virtual void OnCycle(GameMode&  game_mode) {}
	virtual void OnSelect(GameMode& game_mode) {}
protected:
	void ChangeState(GameMode& game_mode, EStates new_state) {
		game_mode.ChangeState(new_state);
	}
};


void GameMode::ChangeState(EStates new_state) {
	states[current_state]->OnExit(*this);
	current_state = new_state;
	states[current_state]->OnExit(*this);
}


int main()
{
	GameMode game;
	game.OnCycle();
	game.OnSelect();
	game.OnCycle();
	game.OnCycle();
	game.OnSelect();
	game.OnCycle();
	game.OnCycle();
	game.OnSelect();
	game.OnCycle();
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

