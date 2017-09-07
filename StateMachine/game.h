#pragma once

#include "fsm.h"
#include "state.h"

class Game
{
private:
	FSM<State, SelectingTankToFire, SelectingTarget> fsm;
public:
	Game() {
		fsm.change_state<SelectingTankToFire>();
	}

	//Event handling:
	void on_cycle();
	void on_select();

	//
	template<class T>
	void change_state() {
		fsm.change_state<T>();
	}
	void to_end_state() {
		fsm.to_end_state();
	}


	//Functions used when transitioning states:
	void HighlightSelectedTank();
	void UnhighlightSelectedTank();
	void IncSelected();

	void SelectObjectivesGroup();
	void HighlightSelectedObjective();
	void UnhighlightSelectedObjective();
	void IncSelectedObjective();
	void AssignDamage();

	//Guards:
	bool MoreTanksToFireExecuted = false;
	bool MoreTanksToFire();
};
