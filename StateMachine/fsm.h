#pragma once

#include "state.h"

//TODO: Instead of on_XXXX() make Event a class and then
//           template <Event> void on<Event>(Event&evt);
class FSM
{
public:
	FSM();
	void on_cycle();
	void on_select();

private:
	using tmap = std::unordered_map<State::state_type, State*>;
	tmap states;
	State::state_type state;

	//TODO: this to a subclass?
	SelectingTankToFire selectingTankToFire;
	SelectingTarget selectingTarget;

public: //TODO: maybe this have to go to a derived class

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

