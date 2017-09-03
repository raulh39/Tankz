#pragma once

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
