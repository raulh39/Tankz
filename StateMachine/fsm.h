#pragma once

#include <iostream>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

class FSM;

class State
{
public:
	using state_type = std::type_index;
	virtual ~State() {}
	virtual void on_enter(FSM &fsm) = 0;
	virtual void on_exit(FSM &fsm) = 0;
	virtual state_type on_cycle(FSM &fsm) = 0;
	virtual state_type on_select(FSM &fsm) = 0;
	
	template <typename T>
	static state_type state() { return typeid(T); }
	
	static state_type end_state() { return typeid(State); }
};

class SelectingTankToFire: public State
{
public:
	virtual void on_enter(FSM &fsm) override;
	virtual void on_exit(FSM &fsm) override;
	virtual state_type on_cycle(FSM &fsm) override;
	virtual state_type on_select(FSM &fsm) override;
};

class SelectingTarget: public State
{
public:
	virtual void on_enter(FSM &fsm) override;
	virtual void on_exit(FSM &fsm) override;
	virtual state_type on_cycle(FSM &fsm) override;
	virtual state_type on_select(FSM &fsm) override;
};

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

